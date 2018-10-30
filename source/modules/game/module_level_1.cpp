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
	if (!cb_gui.create(CB_GUI))
		return false;

	//EngineUI.activateWidget("pantallaCarga");

	cb_gui.pause = 0.f;
	cb_gui.options = 0.f;

	EngineTimer.setTimeSlower(0.f);
	EngineTower.setActualLevelValue("level_1");
	CCamera        camera;
	json jboot = loadJson("data/boot.json");

	// Auto load some scenes
    
    CEntity* e_player = getEntityByName("The Player");
    if (e_player == nullptr) {
        TEntityParseContext ctx;
        parseScene("data/scenes/player.scene", ctx);            
    }
    else {
        TMsgSetFSMVariable turnMsg;
        turnMsg.variant.setName("pause");
        turnMsg.variant.setBool(true);
        e_player->sendMsg(turnMsg);
        TCompCollider* comp_collider = e_player->get<TCompCollider>();
        PxRigidActor* rigidActor = ((PxRigidActor*)comp_collider->actor);           
        TCompTransform* t = e_player->get<TCompTransform>();
        VEC3 pos = VEC3(8.703, 4.5, -30.274);
        t->setYawPitchRoll(deg2rad(-106.039), deg2rad(0));
        t->setPosition(pos);        
        PxTransform tr = rigidActor->getGlobalPose();
        tr.p = PxVec3(8.703, 4.5, -30.274);
        tr.q = PxQuat(-0.000000, -0.798838, 0.000000, 0.601546);
        //rigidActor->setGlobalPose(tr);        
        TMsgSetFSMVariable turnMsg1;
        turnMsg1.variant.setName("idle");
        turnMsg1.variant.setBool(true);
        e_player->sendMsg(turnMsg1);
    }

    TEntityParseContext ctx1;
    parseScene("data/scenes/cameras.scene", ctx1);
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

	cb_globals.global_bajada = 0.f;

	cb_globals.global_fog_percentage_horizontal = 0.15f;
	cb_globals.global_fog_percentage_vertical = 0.15f;

	cb_light.activate();
	cb_object.activate();
	cb_globals.activate();
	cb_camera.activate();
	cb_blur.activate();
	cb_gui.activate();
	cb_particles.activate();

    auto p = EngineScripting.script.exists("OnLevel1Start");
    if (p)
        EngineScripting.script.doString("OnLevel1Start()");

	return true;
}

void CModuleLevel1::restart()
{
   

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

    cb_globals.global_bajada = 0.f;

    cb_globals.global_fog_percentage_horizontal = 0.15f;
    cb_globals.global_fog_percentage_vertical = 0.15f;

    json jboot = loadJson("data/boot.json");

    // Auto load some scenes
    std::vector< std::string > scenes_to_auto_load = jboot["boot_scenes"];
    for (auto& scene_name : scenes_to_auto_load) {
        TEntityParseContext ctx;
        deleteScene(scene_name, ctx);
    }
    std::vector< std::string > scenes_to_auto_load2 = jboot["boot_level_2"];
    for (auto& scene_name : scenes_to_auto_load2) {
        TEntityParseContext ctx;
        deleteScene(scene_name, ctx);
    }
    
    EngineSound.deleteSounds();
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
	deleteScene("data/scenes/scene_Pekes.scene", ctx);
	deleteScene("data/scenes/scene_destruible_1.scene", ctx);
	deleteScene("data/scenes/scene_destruible_2.scene", ctx);
	deleteScene("data/scenes/scene_destruible_3.scene", ctx);
	deleteScene("data/scenes/scene_destruible_4.scene", ctx);
		
    
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
		HWND handle = ::FindWindowEx(0, 0, "MCVWindowsClass", 0);
		//ShowWindow(handle, SW_RESTORE);
		EngineUI.activateSplash();
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

	if (EngineInput[VK_ESCAPE].getsPressed() && !cb_gui.cinematica)
	{
		if (cb_gui.keyboard > 0.f) {
			//EngineTimer.setTimeSlower(1.f);
			//Engine.getModules().changeGameState("test_axis");
			EngineUI.desactiveKeyboardMenu();
			EngineUI.activeOptionMenu();
		}
		else if (cb_gui.options > 0.f) {
			//EngineTimer.setTimeSlower(1.f);
			//Engine.getModules().changeGameState("test_axis");
			EngineUI.desactivateWidget("menu_options");
			EngineUI.desactiveOptionMenu();
			if (cb_gui.main > 0.f) EngineUI.activeMainMenu();
			else EngineUI.activePauseMenu();
		}
		else if(cb_gui.main < 1.f){
			cb_gui.pause -= 1.f;
			if (cb_gui.pause < 0.f)cb_gui.pause = 1.f;
			if (cb_gui.pause > 0.f) {
				EngineUI.activateWidget("pause_menu");
				EngineUI.activePauseMenu();
			}
			else {
				HWND handle = ::FindWindowEx(0, 0, "MCVWindowsClass", 0);
				HCURSOR Cursor = LoadCursorFromFile("data/textures/gui/cursorIngame.cur"); //.cur or .ani
				SetCursor(Cursor);
				SetClassLongPtr(handle, GCLP_HCURSOR, reinterpret_cast<LONG_PTR>(Cursor));
				EngineTimer.setTimeSlower(1.f);
				//Engine.getModules().changeGameState("test_axis");
				EngineUI.desactivateWidget("pause_menu");
				EngineUI.desactivePauseMenu();
				EngineUI.resetPauseMenu();

				CEntity* player = (CEntity*)getEntityByName("The Player");

				TMsgSetFSMVariable pauseMsg;
				pauseMsg.variant.setName("idle");
				pauseMsg.variant.setBool(true);

				player->sendMsg(pauseMsg);
			}
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