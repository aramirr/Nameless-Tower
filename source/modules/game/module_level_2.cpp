#include "mcv_platform.h"
#include "module_level_2.h"
#include "camera/camera.h"
#include "render/render_objects.h"
#include "entity/entity.h"
#include "modules/module_entities.h"
#include "resources/resources_manager.h"
#include "render/texture/texture.h"
#include "render/texture/material.h"
#include "handle/handle.h"
#include "components/juan/comp_name.h"
#include "components/juan/comp_transform.h"
#include "components/camera/comp_camera.h"
#include "components/camera/comp_camera_manager.h"
#include "entity/entity_parser.h"
#include "render/render_manager.h"
#include "scripting\logic_manager.h"


bool CModuleLevel2::start()
{
	json jboot = loadJson("data/boot.json");

	// Auto load some scenes
	std::vector< std::string > scenes_to_auto_load = jboot["boot_level_2"];
	for (auto& scene_name : scenes_to_auto_load) {
	    dbg("Autoloading scene %s\n", scene_name.c_str());
	    TEntityParseContext ctx;
	    parseScene(scene_name, ctx);
	}
    /*
	// -------------------------------------------
	if (!cb_camera.create(CB_CAMERA))
		return false;
	// -------------------------------------------
	if (!cb_object.create(CB_OBJECT))
		return false;
	// -------------------------------------------
	if (!cb_light.create(CB_LIGHT))
		return false;
	// -------------------------------------------
	if (!cb_globals.create(CB_GLOBALS))
		return false;
	if (!cb_blur.create(CB_BLUR))
		return false;
	if (!cb_gui.create(CB_GUI))
	    return false;
	if (!cb_particles.create(CB_PARTICLE))
		return false;

	cb_globals.global_exposure_adjustment = 1.f;
	cb_globals.global_ambient_adjustment = 1.f;
	cb_globals.global_world_time = 0.f;
	cb_globals.global_hdr_enabled = 1.f;
	cb_globals.global_gamma_correction_enabled = 1.f;
	cb_globals.global_tone_mapping_mode = 1.f;

	cb_light.activate();
	cb_object.activate();
	cb_globals.activate();
	cb_camera.activate();
	cb_blur.activate();
    cb_gui.activate();
	cb_particles.activate();
    */
	auto p = EngineScripting.script.exists("OnLevel2Start");
	if (p)
		EngineScripting.script.doString("OnLevel2Start()");

    EngineTower.setExposureAdjustment(1.f);
    return true;
}

bool CModuleLevel2::stop()
{
	cb_light.destroy();
	cb_camera.destroy();
	cb_object.destroy();
	cb_globals.destroy();
	cb_blur.destroy();
    cb_gui.destroy();
	Engine.getEntities().destroyAllEntities();
	Engine.getCameras().destroyAllCameras();
	return true;
}

void CModuleLevel2::update(float delta)
{

	if (carga) {
		CEntity* cam = (CEntity*)getEntityByName("camera_manager");
		TCompCameraManager* cm = cam->get<TCompCameraManager>();
		assert(cm);
    
		carga = false;
	}
  CEntity* cam = (CEntity*)getEntityByName("camera_manager");

  
	static VEC3 world_pos;
	ImGui::DragFloat3("Pos", &world_pos.x, 0.025f, -50.f, 50.f);

	VEC2 mouse = EngineInput.mouse()._position;
	if (h_e_camera.isValid()) {
		CEntity* e_camera = h_e_camera;
		TCompCamera* c_camera = e_camera->get< TCompCamera >();
		VEC3 screen_coords;
		bool inside = c_camera->getScreenCoordsOfWorldCoord(world_pos, &screen_coords);
		ImGui::Text("Inside: %s  Coords: %1.2f, %1.2f  Z:%f", inside ? "YES" : "NO ", screen_coords.x, screen_coords.y, screen_coords.z);
		ImGui::Text("Mouse at %1.2f, %1.2f", mouse.x, mouse.y);
	}

}


void CModuleLevel2::render()
{

  // Render the grid
  cb_object.obj_world = MAT44::Identity;
  cb_object.obj_color = VEC4(1,1,1,1);
  cb_object.updateGPU();
 
}