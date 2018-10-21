#include "mcv_platform.h"
#include "module_level_1.h"
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


bool CModuleLevel1::start()
{
	EngineTimer.setTimeSlower(0.f);
	CCamera        camera;
	json jboot = loadJson("data/boot.json");

	// Auto load some scenes
	std::vector< std::string > scenes_to_auto_load = jboot["boot_scenes"];
	for (auto& scene_name : scenes_to_auto_load) {
		TEntityParseContext ctx;
		parseScene(scene_name, ctx);
	}

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

	cb_globals.global_exposure_adjustment = 0.440f;
	cb_globals.global_ambient_adjustment = 1.f;
	//cb_globals.global_exposure_adjustment = 2.010f;
	//cb_globals.global_ambient_adjustment = 0.150f;
	cb_globals.global_world_time = 0.f;
	cb_globals.global_hdr_enabled = 1.f;
	cb_globals.global_gamma_correction_enabled = 1.f;
	cb_globals.global_tone_mapping_mode = 1.f;
	cb_globals.global_naja_interior = 0;
	//cb_globals.global_fog_density = 0.017f;
	//cb_globals.global_self_intensity = 10.f;
	cb_globals.global_hue_adjustment = 1.f;
	cb_globals.global_sat_adjustment = 1.f;
	cb_globals.global_light_adjustment = 0.f;
	cb_globals.global_vignetting_adjustment = 0.25f;
	cb_globals.global_fogDensity_adjustment = 0.f;
	cb_globals.global_fadeOut_adjustment = 0.f;

	cb_globals.global_contrast_adjustment = 0.215f;

	cb_globals.global_bandMin_adjustment = 0.f;
	cb_globals.global_bandMax_adjustment = 0.f;

	cb_globals.global_fog_percentage_horizontal = 0.15f;
	cb_globals.global_fog_percentage_vertical = 0.15f;

	cb_light.activate();
	cb_object.activate();
	cb_globals.activate();
	cb_camera.activate();
	cb_blur.activate();
	cb_gui.activate();
	cb_particles.activate();

	return true;
}

bool CModuleLevel1::stop()
{	
	TEntityParseContext ctx;
    EngineBillboards.clearFire();
		deleteScene("data/scenes/scene_checkpoints.scene", ctx);
    deleteScene("data/scenes/lights.scene", ctx);
    deleteScene("data/scenes/scene_luces.scene", ctx);
    deleteScene("data/scenes/compresoras.scene", ctx);
    deleteScene("data/scenes/spline_elements.scene", ctx);
    deleteScene("data/scenes/scene_parte1_colltrig.scene", ctx);
    deleteScene("data/scenes/scene_parte1_logic.scene", ctx);
    deleteScene("data/scenes/scene_parte1_meshes.scene", ctx);
    deleteScene("data/scenes/scene_parte2_colltrig.scene", ctx);
    deleteScene("data/scenes/scene_parte2_logic.scene", ctx);
    deleteScene("data/scenes/scene_parte2_meshes.scene", ctx);
    deleteScene("data/scenes/scene_parte3_colltrig.scene", ctx);
    deleteScene("data/scenes/scene_parte3_logic.scene", ctx);
    deleteScene("data/scenes/scene_parte3_meshes.scene", ctx);
    deleteScene("data/scenes/scene_parte3_colltrig.scene", ctx);
    deleteScene("data/scenes/particles.scene", ctx);
		deleteScene("data/scenes/torch_puzzle.scene", ctx);
		
    
	return true;
}

void CModuleLevel1::update(float delta)
{

	if (carga) {
		CEntity* cam = (CEntity*)getEntityByName("camera_manager");
		TCompCameraManager* cm = cam->get<TCompCameraManager>();
		assert(cm);

		//cm->activateCinematic("final");

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
		pausa = !pausa;
		if (pausa) {
			EngineUI.activateWidget("menu_pausa");
			EngineUI.activePauseMenu();
		}
		else {
			EngineTimer.setTimeSlower(1.f);
			//Engine.getModules().changeGameState("test_axis");
			EngineUI.desactivateWidget("menu_pausa");
			EngineUI.desactivePauseMenu();

			CEntity* player = (CEntity*)getEntityByName("The Player");

			TMsgSetFSMVariable pauseMsg;
			pauseMsg.variant.setName("idle");
			pauseMsg.variant.setBool(true);

			player->sendMsg(pauseMsg);
		}
	}

}


void CModuleLevel1::render()
{
	// Render the grid
	cb_object.obj_world = MAT44::Identity;
	cb_object.obj_color = VEC4(1, 1, 1, 1);
	cb_object.updateGPU();	
}