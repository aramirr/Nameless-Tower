#include "mcv_platform.h"
#include "comp_light_point.h"
#include "components/juan/comp_transform.h"
#include "ctes.h"                     // texture slots
#include "render/render_manager.h" 
#include "render/render_objects.h" 
#include "render/texture/render_to_cube.h"

DECL_OBJ_MANAGER("light_point", TCompLightPoint);

DXGI_FORMAT readFormat(const json& j, const std::string& label);

// -------------------------------------------------
void TCompLightPoint::debugInMenu() {
	ImGui::DragFloat("Intensity", &intensity, 0.01f, 0.f, 10.f);
	ImGui::ColorEdit3("Color", &color.x);
	ImGui::DragFloat("Radius", &radius, 0.01f, 0.f, 100.f);
}

MAT44 TCompLightPoint::getWorld() {
	TCompTransform* c = get<TCompTransform>();
	if (!c)
		return MAT44::Identity;
	return MAT44::CreateScale(radius) * c->asMatrix();
}

// -------------------------------------------------
void TCompLightPoint::renderDebug() {
	// Render a wire sphere
	auto mesh = Resources.get("data/meshes/UnitSphere.mesh")->as<CRenderMesh>();
	renderMesh(mesh, getWorld(), VEC4(1, 1, 1, 1));
}

// -------------------------------------------------
void TCompLightPoint::load(const json& j, TEntityParseContext& ctx) {
	if (j.count("color"))
		color = loadVEC4(j["color"]);
	intensity = j.value("intensity", intensity);
	radius = j.value("radius", radius);

	// Check if we need to allocate a shadow map
	casts_shadows = j.value("casts_shadows", false);
	if (casts_shadows) {
		shadows_step = j.value("shadows_step", shadows_step);
		shadows_resolution = j.value("shadows_resolution", shadows_resolution);
		auto shadowmap_fmt = readFormat(j, "shadows_fmt");
		assert(shadows_resolution > 0);
		shadows_rt = new CRenderToCube;
		// Make a unique name to have the Resource Manager happy with the unique names for each resource
		char my_name[64];
		sprintf(my_name, "shadow_map_%08x", CHandle(this).asUnsigned());

		// Added a placeholder Color Render Target to be able to do a alpha test when rendering
		// the grass
		bool is_ok = shadows_rt->create(my_name, shadows_resolution, DXGI_FORMAT_R8G8B8A8_UNORM, shadowmap_fmt);
		//bool is_ok = shadows_rt->createRT(my_name, shadows_resolution, shadows_resolution, DXGI_FORMAT_R8G8B8A8_UNORM, shadowmap_fmt);
		assert(is_ok);
	}

	shadows_enabled = casts_shadows;

	this->setPerspective(deg2rad(37.3f), 2.0f, 160.0f);
}

void TCompLightPoint::update(float dt) {
	// Can't use the TCompCamera::update because inside it calls 
	// get<TCompTransform> which tries to convert 'this' to an instance
	// of TCompCamera, but will fail because we are a CompLightDir
	TCompTransform* c = get<TCompTransform>();
	if (!c)
		return;
	//shadows_rt->setPosition(c->getPosition());
	//this->lookAt(c->getPosition(), c->getPosition() + c->getFront(), c->getUp());
}

// -------------------------------------------------
// Updates the Shader Cte Light with MY information
void TCompLightPoint::activate(int i) {

	// To avoid converting the range -1..1 to 0..1 in the shader
	// we concatenate the view_proj with a matrix to apply this offset
	if (shadows_rt)shadows_rt->getCamera(i, this);
	TCompTransform* c = get<TCompTransform>();
	if (!c)
		return;


	MAT44 mtx_offset = MAT44::CreateScale(VEC3(0.5f, -0.5f, 1.0f))
		* MAT44::CreateTranslation(VEC3(0.5f, 0.5f, 0.0f));

	cb_light.light_color = color;
	cb_light.light_intensity = intensity;
	cb_light.light_pos = c->getPosition();
	cb_light.light_radius = radius * c->getScale();
	cb_light.light_view_proj_offset = getViewProjection() * mtx_offset;
	cb_light.light_direction = VEC4(c->getFront().x, c->getFront().y, c->getFront().z, 1);
	cb_light.light_point = 1;
	cb_light.light_angle = 90;
	cb_light.updateGPU();

	// If we have a ZTexture, it's the time to activate it
	if (shadows_rt) {

		cb_light.light_shadows_inverse_resolution = 1.0f / (float)shadows_rt->getWidth();
		cb_light.light_shadows_step = shadows_step;
		cb_light.light_shadows_step_with_inv_res = shadows_step / (float)shadows_rt->getWidth();
		//cb_light.light_radius = 1.f;

		assert(shadows_rt->getZTexture());
		shadows_rt->getZTexture()->activate(TS_LIGHT_SHADOW_MAP);

	}
}

// ------------------------------------------------------
void TCompLightPoint::generateShadowMap() {
	if (!shadows_rt || !shadows_enabled)
		return;

	TCompTransform* c = get<TCompTransform>();
	if (!c)
		return;
	this->lookAt(c->getPosition(), VEC3(0,0,0), VEC3(0,1,0));

	// In this slot is where we activate the render targets that we are going
	// to update now. You can't be active as texture and render target at the
	// same time
	

	for (int i = 0; i < 6; i++) {
		CTexture::setNullTexture(TS_LIGHT_SHADOW_MAP);
		CTraceScoped gpu_scope(shadows_rt->getName().c_str());
		shadows_rt->activateFace(i, this);
		{
			PROFILE_FUNCTION("Clear&SetCommonCtes");
			shadows_rt->clearDepthBuffer();
			// We are going to render the scene from the light position & orientation
			activateCamera(*this, shadows_rt->getWidth(), shadows_rt->getHeight());
		}
		//CRenderManager::get().setEntityCamera(CHandle(this).getOwner());
		CRenderManager::get().renderCategory("shadows");
	}
}

void TCompLightPoint::setIntensity(float value) {
	intensity = value;
}

