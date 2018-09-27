#include "mcv_platform.h"
#include "comp_light_dir.h"
#include "components/juan/comp_transform.h"
#include "render/render_objects.h"    // cb_light
#include "render/texture/texture.h" 
#include "ctes.h"                     // texture slots
#include "render/texture/render_to_texture.h" 
#include "render/render_manager.h" 
#include "components/juan/comp_culling.h"
#include "entity/entity_parser.h"

DECL_OBJ_MANAGER("light_dir", TCompLightDir);

DXGI_FORMAT readFormat(const json& j, const std::string& label);

// -------------------------------------------------
void TCompLightDir::debugInMenu() {
	TCompCamera::debugInMenu();
	ImGui::DragFloat("Intensity", &intensity, 0.01f, 0.f, 75.f);
	ImGui::ColorEdit3("Color", &color.x);
	ImGui::DragFloat("Step Size", &shadows_step, 0.01f, 0.f, 5.f);
}

// -------------------------------------------------
void TCompLightDir::renderDebug() {
  TCompCamera::renderDebug();
}

// -------------------------------------------------
void TCompLightDir::load(const json& j, TEntityParseContext& ctx) {
	setEntity(ctx.current_entity);
  TCompCamera::load( j, ctx );
  if(j.count("color"))
    color = loadVEC4(j["color"]);
	intensity = j.value("intensity", intensity);
	has_culling = j.value("has_culling", false);

  if( j.count("projector")) {
    std::string projector_name = j.value("projector", "");
    projector = Resources.get(projector_name)->as<CTexture>();
  } else {
		projector = Resources.get("data/textures/white.dds")->as<CTexture>();
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

void TCompLightDir::update(float dt) {
  // Can't use the TCompCamera::update because inside it calls 
  // get<TCompTransform> which tries to convert 'this' to an instance
  // of TCompCamera, but will fail because we are a CompLightDir
  TCompTransform* c = get<TCompTransform>();
  if (!c)
    return;
  this->lookAt(c->getPosition(), c->getPosition() + c->getFront(), c->getUp());
}

// -------------------------------------------------
// Updates the Shader Cte Light with MY information
void TCompLightDir::activate() {
  TCompTransform* c = get<TCompTransform>();
  if (!c)
    return;

  if (projector)
    projector->activate( TS_LIGHT_PROJECTOR );

  // To avoid converting the range -1..1 to 0..1 in the shader
  // we concatenate the view_proj with a matrix to apply this offset
  MAT44 mtx_offset = MAT44::CreateScale(VEC3(0.5f, -0.5f, 1.0f))
                   * MAT44::CreateTranslation(VEC3(0.5f, 0.5f, 0.0f));

  cb_light.light_color = color;
  cb_light.light_intensity = intensity;
  cb_light.light_pos = c->getPosition();
  cb_light.light_radius = getZFar();
  cb_light.light_view_proj_offset = getViewProjection() * mtx_offset;
  cb_light.light_direction = VEC4(c->getFront().x, c->getFront().y, c->getFront().z, 1);
  cb_light.light_point = 0;
  cb_light.light_angle = 0;
  cb_light.updateGPU();

  // If we have a ZTexture, it's the time to activate it
  if (shadows_rt && intensity != 0) {

    cb_light.light_shadows_inverse_resolution = 1.0f / (float)shadows_rt->getWidth();
    cb_light.light_shadows_step = shadows_step;
    cb_light.light_shadows_step_with_inv_res = shadows_step / (float)shadows_rt->getWidth();
    //cb_light.light_radius = 1.f;

    assert(shadows_rt->getZTexture());
    shadows_rt->getZTexture()->activate(TS_LIGHT_SHADOW_MAP);
  }
  /*else {
    CTexture::setNullTexture( TS_LIGHT_SHADOW_MAP );
  }*/

}



// ------------------------------------------------------
void TCompLightDir::generateShadowMap() {
  if (!shadows_rt || !shadows_enabled || intensity == 0)
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

	//�?CRenderManager::get().setEntityCamera(CHandle(this).getOwner());
	TCompCulling* culling;
	if (has_culling) {
		CEntity* e = h_entity;
		culling = e->get<TCompCulling>();
	}
	else culling = nullptr;

	CRenderManager::get().renderCategory("shadows", culling);  
}

void TCompLightDir::setIntensity(float value) {
  intensity = value;
}

void TCompLightDir::setEntity(CHandle new_entity) {
	h_entity = new_entity;
	assert(h_entity.isValid());
}

