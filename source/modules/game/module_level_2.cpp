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
 	    TEntityParseContext ctx;
	    parseScene(scene_name, ctx);
	}

	auto p = EngineScripting.script.exists("OnLevel2Start");
	if (p)
		EngineScripting.script.doString("OnLevel2Start()");

	cb_globals.global_bajada = 1.f;

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

  if (EngineInput[VK_ESCAPE].getsPressed())
  {
		if (cb_gui.options > 0.f) {
			EngineTimer.setTimeSlower(1.f);
			//Engine.getModules().changeGameState("test_axis");
			EngineUI.desactivateWidget("menu_options");
			EngineUI.desactiveOptionMenu();
		}
		else if (cb_gui.main < 1.f) {
			cb_gui.pause -= 1.f;
			if (cb_gui.pause < 0.f)cb_gui.pause = 1.f;
			if (cb_gui.pause > 0.f) {
				EngineUI.activateWidget("pause_menu"); 
				EngineUI.activePauseMenu();
			}
			else {
				EngineTimer.setTimeSlower(1.f);
				//Engine.getModules().changeGameState("test_axis");
				EngineUI.desactivateWidget("pause_menu");
				EngineUI.desactivePauseMenu();

				CEntity* player = (CEntity*)getEntityByName("The Player");

				TMsgSetFSMVariable pauseMsg;
				pauseMsg.variant.setName("idle");
				pauseMsg.variant.setBool(true);

				player->sendMsg(pauseMsg);
			}
		}
  }

}


void CModuleLevel2::render()
{

  // Render the grid
  cb_object.obj_world = MAT44::Identity;
  cb_object.obj_color = VEC4(1,1,1,1);
  cb_object.updateGPU();
 
}