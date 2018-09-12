#include "mcv_platform.h"
#include "turn_around_state.h"
#include "fsm/context.h"
#include "entity/entity_parser.h"
#include "components/player/comp_player_controller.h"

namespace FSM
{
	void TurnAroundState::onStart(CContext& ctx) const
	{					
		CEntity* e = ctx.getOwner();
		TCompPlayerController* player = e->get<TCompPlayerController>();
		//player->change_animation(player->EAnimations::NajaBreak, _is_action, _delay_in, _delay_out);
	}
	
	bool TurnAroundState::load(const json& jData)
	{		
		_is_action = jData.value("is_action", false);
		_delay_out = jData.value("delay_out", 0.01f);
		_delay_in = jData.value("delay_in", 0.01f);
		return true;
	}

	bool TurnAroundState::update(float dt, CContext& ctx) const
	{		
        /*CEntity* e = ctx.getOwner();
        TCompTransform *c_my_transform = e->get<TCompTransform>();
        float current_yaw;
        float current_pitch;
        c_my_transform->getYawPitchRoll(&current_yaw, &current_pitch);

        VEC3 myPos = c_my_transform->getPosition();
        current_yaw = current_yaw - 0.1f * dt;
        c_my_transform->setYawPitchRoll(current_yaw, current_pitch);
		*/return false;
	}

	void TurnAroundState::onFinish(CContext& ctx) const {
		ctx.setVariable("turn_around", false);
        CEntity* e = ctx.getOwner();
        TCompPlayerController* player = e->get<TCompPlayerController>();
        player->previous_state = "turn_around";
	}
}

