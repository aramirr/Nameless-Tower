#include "mcv_platform.h"
#include "components/juan/comp_transform.h"
#include "components/controllers/comp_curve.h"
#include "comp_camera_manager.h"
#include "entity/entity_parser.h"
#include "comp_cinematic.h"

DECL_OBJ_MANAGER("cinematic", TCompCinematic);

void TCompCinematic::load(const json& j, TEntityParseContext& ctx) {
  CEntity* e = ctx.current_entity;
	time = j.value("time", 0.0f);
	blend_time = j.value("time", 2.0f);
	blend_time = j.value("time", 2.0f);
	spline_name = j.value("spline_name", "");
	camera_name = j.value("camera_name", "");
}


void TCompCinematic::update(float dt) {
  // Advance the time
	if (active) {
		current_time += dt;
		if (time != 0 && current_time > time) {
			deactivate();
			active = false;
			current_time = 0;
		}
	} 
}

void TCompCinematic::activate(const TMsgActivateCinematic& msg) {
    if (!active) {
        CEntity* camera_manager = (CEntity*)getEntityByName("camera_manager");
        TMsgActiveCamera activate_camera;
        activate_camera.camera_name = camera_name;
        activate_camera.blend_time = 2.f;
        camera_manager->sendMsg(activate_camera);
        if (spline_name != "") {
            CEntity* h_spline = (CEntity*)getEntityByName(spline_name);
            TCompCurve* c = h_spline->get<TCompCurve>();
            c->activate();
        }
        active = true;
    }	
}

void TCompCinematic::deactivate_msg(const TMsgDeactivateCinematic& msg) {
    deactivate();
}

void TCompCinematic::deactivate() {
    if (active){
        CEntity* camera_manager = (CEntity*)getEntityByName("camera_manager");
        TMsgActiveCamera activate_camera;
        activate_camera.camera_name = "camera_orbit_IZQ";
        activate_camera.blend_time = 2.f;
        camera_manager->sendMsg(activate_camera);
        if (spline_name != "") {
            CEntity* h_spline = (CEntity*)getEntityByName(spline_name);
            TCompCurve* c = h_spline->get<TCompCurve>();
            c->deactivate();
        }
        active = false;
    }	
}

void TCompCinematic::registerMsgs() {
    DECL_MSG(TCompCinematic, TMsgActivateCinematic, activate);
    DECL_MSG(TCompCinematic, TMsgDeactivateCinematic, deactivate_msg);
}