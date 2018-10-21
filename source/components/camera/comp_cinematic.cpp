#include "mcv_platform.h"
#include "components/juan/comp_transform.h"
#include "components/controllers/comp_curve.h"
#include "components/player/comp_player_controller.h"
#include "comp_camera_manager.h"
#include "entity/entity_parser.h"
#include "comp_cinematic.h"

DECL_OBJ_MANAGER("cinematic", TCompCinematic);

void TCompCinematic::load(const json& j, TEntityParseContext& ctx) {
  CEntity* e = ctx.current_entity;
  auto& splines = j["splines"];
  for (auto it = splines.begin(); it != splines.end(); ++it) {
      Cinematic cine;
      cine.time = it.value().value("time", 0.0f);
      cine.blend_time = it.value().value("blend_time", 2.0f);
      cine.spline_name = it.value().value("spline_name", "");
      cine.camera_name = it.value().value("camera_name", "");
      cinematics.push_back(cine);
  }
  current_spline = 0;  
	current_time = 0;
}


void TCompCinematic::update(float dt) {
  // Advance the time
	current_time += dt;
	if (active) {		
		if (time != 0 && current_time > time) {
            current_spline += 1;
            if (current_spline == cinematics.size()) {
                current_spline -= 1;
                deactivate();
                active = false;
                current_time = 0;
            }
            else {
                current_time = 0;
                time = cinematics[current_spline].time;
                TMsgActiveCamera activate_camera;
                activate_camera.camera_name = cinematics[current_spline].camera_name;
                activate_camera.blend_time = cinematics[current_spline].blend_time;
                CEntity* camera_manager = (CEntity*)getEntityByName("camera_manager");
                camera_manager->sendMsg(activate_camera);
                CEntity* h_spline = (CEntity*)getEntityByName(cinematics[current_spline].spline_name);
                TCompCurve* c = h_spline->get<TCompCurve>();
                c->activate();
            }
			
		}
	} 
}

void TCompCinematic::activate(const TMsgActivateCinematic& msg) {
    if (!active) {
        CEntity* camera_manager = (CEntity*)getEntityByName("camera_manager");
        CEntity* e_player = (CEntity*)getEntityByName("The Player");
        TCompPlayerController* player = e_player->get<TCompPlayerController>();
        player->on_cinematic = true;
        player->previous_camera = cinematics[0].camera_name;
        TMsgActiveCamera activate_camera;
        activate_camera.camera_name = cinematics[0].camera_name;
        activate_camera.blend_time = cinematics[0].blend_time;
        camera_manager->sendMsg(activate_camera);
        time = cinematics[0].time;
        current_spline = 0;
				current_time = 0;
        if (cinematics[0].spline_name != "") {
            CEntity* h_spline = (CEntity*)getEntityByName(cinematics[0].spline_name);
            TCompCurve* c = h_spline->get<TCompCurve>();            
            c->activate();
        }
        active = true;
        TMsgSetFSMVariable cinematicMsg;
        cinematicMsg.variant.setName("on_cinematic");
        cinematicMsg.variant.setBool(true);
        e_player->sendMsg(cinematicMsg);
    }	
}

void TCompCinematic::deactivate_msg(const TMsgDeactivateCinematic& msg) {
    deactivate();
}

void TCompCinematic::deactivate() {
    if (active){
        CEntity* e_player = (CEntity*)getEntityByName("The Player");
        TCompPlayerController* player = e_player->get<TCompPlayerController>();
        player->on_cinematic = false;
        CEntity* camera_manager = (CEntity*)getEntityByName("camera_manager");
        TMsgActiveCamera activate_camera;
        activate_camera.camera_name = "camera_orbit_IZQ";
        activate_camera.blend_time = 2.f;
        camera_manager->sendMsg(activate_camera);
        if (cinematics[current_spline].spline_name != "") {
            CEntity* h_spline = (CEntity*)getEntityByName(cinematics[current_spline].spline_name);
            TCompCurve* c = h_spline->get<TCompCurve>();
            c->deactivate();
        }
        active = false;
        TMsgSetFSMVariable cinematicMsg;
        cinematicMsg.variant.setName("on_cinematic");
        cinematicMsg.variant.setBool(false);
        e_player->sendMsg(cinematicMsg);
    }	
}

void TCompCinematic::registerMsgs() {
    DECL_MSG(TCompCinematic, TMsgActivateCinematic, activate);
    DECL_MSG(TCompCinematic, TMsgDeactivateCinematic, deactivate_msg);
}