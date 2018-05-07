#include "mcv_platform.h"
#include "landing_state.h"
#include "fsm/context.h"
#include "components/player/comp_player_controller.h"
#include "components/fsm/comp_fsm.h"
#include "components/physics/controller_filter.h"

namespace FSM
{
	void LandingState::onStart(CContext& ctx) const
	{		
		CEntity* e = ctx.getOwner();
		TCompPlayerController* player = e->get<TCompPlayerController>();
		//player->change_animation(player->EAnimations::NajaJumpLand, _is_action, _delay_in, _delay_out);
		TMsgSetFSMVariable fallingMsg;
	}

	bool LandingState::load(const json& jData)
	{
		_is_action = jData.value("is_action", false);
		_delay_out = jData.value("delay_out", 0.01f);
		_delay_in = jData.value("delay_in", 0.01f);
		return true;
	}

	bool LandingState::update(float dt, CContext& ctx) const
	{		
		return false;
	}

	void LandingState::onFinish(CContext& ctx) const {
	}

}

