#include "mcv_platform.h"
#include "dying_state.h"
#include "fsm/context.h"
#include "components/player/comp_player_controller.h"
#include "components/fsm/comp_fsm.h"
#include "components/physics/controller_filter.h"

namespace FSM
{
	void DyingState::onStart(CContext& ctx) const
	{
		CEntity* e = ctx.getOwner();
		TCompPlayerController* player = e->get<TCompPlayerController>();
		player->change_animation(player->EAnimations::NajaDying, _is_action, _delay_in, _delay_out);
	}

	bool DyingState::load(const json& jData)
	{
		_is_action = jData.value("is_action", true);
		_delay_out = jData.value("delay_out", 0.01f);
		_delay_in = jData.value("delay_out", 0.01f);
		return true;
	}

	bool DyingState::update(float dt, CContext& ctx) const
	{		
		return false;
	}
	
	void DyingState::onFinish(CContext& ctx) const {		
	}
}