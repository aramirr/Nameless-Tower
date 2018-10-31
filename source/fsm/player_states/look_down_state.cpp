#include "mcv_platform.h"
#include "look_down_state.h"
#include "fsm/context.h"
#include "components/player/comp_player_controller.h"
#include "components/fsm/comp_fsm.h"
#include "components/physics/controller_filter.h"
#include "components/physics/query_filter.h"


namespace FSM
{
	void LookDownState::onStart(CContext& ctx) const
	{
		CEntity* e = ctx.getOwner();
		TCompPlayerController* player = e->get<TCompPlayerController>();
		_sound->setVolume(player->volumen);
		player->change_animation(player->EAnimations::NajaLookDn, _is_action, _delay_in, _delay_out, true);
        CEntity* camera_manager = (CEntity*)getEntityByName("camera_manager");
        TMsgActiveCamera activate_camera;
        activate_camera.camera_name = "camera_look_down";
        activate_camera.blend_time = 4.f;
        camera_manager->sendMsg(activate_camera);
	}

	bool LookDownState::load(const json& jData)
	{
		_is_action = jData.value("is_action", false);
		_delay_out = jData.value("delay_out", 0.01f);
		_delay_in = jData.value("delay_in", 0.01f);
		return true;
	}

	bool LookDownState::update(float dt, CContext& ctx) const
	{		
        if (!EngineInput["look_down"].isPressed())
        {
            ctx.setVariable("idle", true);
        }        
        
		return false;
	}

	void LookDownState::onFinish(CContext& ctx) const {
        ctx.setVariable("look_down", false);
        CEntity* e = ctx.getOwner();
        TCompPlayerController* player = e->get<TCompPlayerController>();
        CEntity* camera_manager = (CEntity*)getEntityByName("camera_manager");
        TMsgRemoveCamera deactivate_camera;
        deactivate_camera.camera_name = "camera_look_down";
        deactivate_camera.blend_time = 6.f;
        camera_manager->sendMsg(deactivate_camera);
        TMsgActiveCamera activate_camera;
        activate_camera.camera_name = "camera_orbit_IZQ";
        activate_camera.blend_time = 4.f;
        camera_manager->sendMsg(activate_camera);
        player->previous_state = "look_down";
	}
}