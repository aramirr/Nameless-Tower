#include "mcv_platform.h"
#include "look_up_state.h"
#include "fsm/context.h"
#include "components/player/comp_player_controller.h"
#include "components/fsm/comp_fsm.h"
#include "components/physics/controller_filter.h"
#include "components/physics/query_filter.h"


namespace FSM
{
	void LookUpState::onStart(CContext& ctx) const
	{
		CEntity* e = ctx.getOwner();
		TCompPlayerController* player = e->get<TCompPlayerController>();
		player->change_animation(player->EAnimations::NajaLookUp, _is_action, _delay_in, _delay_out, true);
        CEntity* camera_manager = (CEntity*)getEntityByName("camera_manager");
        TMsgActiveCamera activate_camera;
        activate_camera.camera_name = "camera_look_up";
        activate_camera.blend_time = 4.f;
        camera_manager->sendMsg(activate_camera);        
	}

	bool LookUpState::load(const json& jData)
	{
		_is_action = jData.value("is_action", false);
		_delay_out = jData.value("delay_out", 0.01f);
		_delay_in = jData.value("delay_in", 0.01f);
		return true;
	}

	bool LookUpState::update(float dt, CContext& ctx) const
	{		
        if (!EngineInput["look_up"].isPressed())
        {
            ctx.setVariable("idle", true);
        }        
        
		return false;
	}

	void LookUpState::onFinish(CContext& ctx) const {
        ctx.setVariable("look_up", false);
        CEntity* e = ctx.getOwner();
        TCompPlayerController* player = e->get<TCompPlayerController>();
        CEntity* camera_manager = (CEntity*)getEntityByName("camera_manager");
        TMsgRemoveCamera deactivate_camera;
        deactivate_camera.camera_name = "camera_look_up";
        deactivate_camera.blend_time = 6.f;
        camera_manager->sendMsg(deactivate_camera);
        TMsgActiveCamera activate_camera;
        activate_camera.camera_name = "camera_orbit_IZQ";
        activate_camera.blend_time = 4.f;
        camera_manager->sendMsg(activate_camera);
        player->previous_state = "landing";
        
	}
}