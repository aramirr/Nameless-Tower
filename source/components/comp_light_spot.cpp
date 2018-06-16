#include "mcv_platform.h"
#include "comp_light_spot.h"
#include "components/juan/comp_transform.h"
#include "render/render_objects.h"    // cb_light
#include "render/texture/texture.h" 
#include "ctes.h"                     // texture slots
#include "render/texture/render_to_texture.h" 
#include "render/render_manager.h" 

DECL_OBJ_MANAGER("light_spot", TCompLightSpot);

//DXGI_FORMAT readFormat(const json& j, const std::string& label);

// -------------------------------------------------
void TCompLightSpot::debugInMenu() {
	TCompCamera::debugInMenu();
	ImGui::DragFloat("Intensity", &intensity, 0.01f, 0.f, 10.f);
	ImGui::ColorEdit3("Color", &color.x);
	ImGui::DragFloat("Step Size", &shadows_step, 0.01f, 0.f, 5.f);
	ImGui::DragFloat("Angle", &angle, 0.5f, 1.f, 160.f);
	ImGui::DragFloat("Cut Out", &inner_cut, 0.5f, 1.f, angle);
	ImGui::DragFloat("Range", &range, 0.5f, 1.f, 120.f);
}

// -------------------------------------------------
void TCompLightSpot::renderDebug() {
  TCompCamera::renderDebug();

	// Render a wire sphere
	//auto mesh = Resources.get("data/meshes/UnitCone.mesh")->as<CRenderMesh>();
	//renderMesh(mesh, getWorld(), VEC4(1, 1, 1, 1));
}

// -------------------------------------------------
void TCompLightSpot::load(const json& j, TEntityParseContext& ctx) {
  TCompCamera::load( j, ctx );
  if(j.count("color"))
    color = loadVEC4(j["color"]);
  intensity = j.value("intensity", intensity);

	angle = j.value("angle", 45.f);
	range = j.value("range", 10.f);
	inner_cut = j.value("inner_cut", angle);
	outer_cut = j.value("outer_cut", angle);

  if( j.count("projector")) {
    std::string projector_name = j.value("projector", "");
    projector = Resources.get(projector_name)->as<CTexture>();
  } else {
		projector = Resources.get("data/textures/default_white.dds")->as<CTexture>();
	}

  // Check if we need to allocate a shadow map
  casts_shadows = j.value("casts_shadows", false);
  if (casts_shadows) {
    shadows_step = j.value("shadows_step", shadows_step);
    shadows_resolution = j.value("shadows_resolution", shadows_resolution);
    auto shadowmap_fmt = readFormat(j, "shadows_fmt");
    assert(shadows_resolution > 0);
    shadows_rt = new CRenderToTexture;
    // Make a unique name to have the Resource Manager happy with the unique names for each resource
    char my_name[64];
    sprintf(my_name, "shadow_map_%08x", CHandle(this).asUnsigned());
    
    // Added a placeholder Color Render Target to be able to do a alpha test when rendering
    // the grass
    bool is_ok = shadows_rt->createRT(my_name, shadows_resolution, shadows_resolution, DXGI_FORMAT_R8G8B8A8_UNORM, shadowmap_fmt);
    assert(is_ok);
  }

  shadows_enabled = casts_shadows;
}

void TCompLightSpot::setColor(const VEC4 & new_color) {

	color = new_color;
}

MAT44 TCompLightSpot::getWorld() {

	TCompTransform* c = get<TCompTransform>();
	if (!c)
		return MAT44::Identity;

	float new_scale = tan(deg2rad(angle * .5f)) * range;
	return MAT44::CreateScale(VEC3(new_scale, new_scale, range)) * c->asMatrix();
}

void TCompLightSpot::update(float dt) {
  // Can't use the TCompCamera::update because inside it calls 
  // get<TCompTransform> which tries to convert 'this' to an instance
  // of TCompCamera, but will fail because we are a CompLightDir
  TCompTransform* c = get<TCompTransform>();
  if (!c)
    return;
  this->lookAt(c->getPosition(), c->getPosition() + c->getFront(), c->getUp());
	this->setPerspective(deg2rad(angle), 0.1f, range); // might change this znear in the future, hardcoded for clipping purposes.
}

// -------------------------------------------------
// Updates the Shader Cte Light with MY information
void TCompLightSpot::activate() {
  TCompTransform* c = get<TCompTransform>();
  if (!c)
    return;

  if (projector)
    projector->activate( TS_LIGHT_PROJECTOR );

  // To avoid converting the range -1..1 to 0..1 in the shader
  // we concatenate the view_proj with a matrix to apply this offset
  MAT44 mtx_offset = MAT44::CreateScale(VEC3(0.5f, -0.5f, 1.0f))
                   * MAT44::CreateTranslation(VEC3(0.5f, 0.5f, 0.0f));

	float spot_angle = cos(deg2rad(angle * .5f));
	cb_light.light_color = color;
	cb_light.light_intensity = intensity;
	cb_light.light_pos = c->getPosition();
	cb_light.light_radius = range * c->getScale();
	cb_light.light_view_proj_offset = getViewProjection() * mtx_offset;
	cb_light.light_angle = spot_angle;
	cb_light.light_direction = VEC4(c->getFront().x, c->getFront().y, c->getFront().z, 1);
	cb_light.light_inner_cut = cos(deg2rad(clamp(inner_cut, 0.f, angle) * .5f));
	cb_light.light_outer_cut = spot_angle;
  cb_light.updateGPU();

  // If we have a ZTexture, it's the time to activate it
  if (shadows_rt) {

    cb_light.light_shadows_inverse_resolution = 1.0f / (float)shadows_rt->getWidth();
    cb_light.light_shadows_step = shadows_step;
    cb_light.light_shadows_step_with_inv_res = shadows_step / (float)shadows_rt->getWidth();
    cb_light.light_radius = range;

    assert(shadows_rt->getZTexture());
    shadows_rt->getZTexture()->activate(TS_LIGHT_SHADOW_MAP);
  }

}



// ------------------------------------------------------
void TCompLightSpot::generateShadowMap() {
  if (!shadows_rt || !shadows_enabled )
    return;

  // In this slot is where we activate the render targets that we are going
  // to update now. You can't be active as texture and render target at the
  // same time
  CTexture::setNullTexture(TS_LIGHT_SHADOW_MAP);

  CTraceScoped gpu_scope(shadows_rt->getName().c_str());
  shadows_rt->activateRT();

  {
    PROFILE_FUNCTION("Clear&SetCommonCtes");
    shadows_rt->clearZ();
    // We are going to render the scene from the light position & orientation
    activateCamera(*this, shadows_rt->getWidth(), shadows_rt->getHeight());
  }

	//¿?CRenderManager::get().setEntityCamera(CHandle(this).getOwner());
  CRenderManager::get().renderCategory("shadows");
}




