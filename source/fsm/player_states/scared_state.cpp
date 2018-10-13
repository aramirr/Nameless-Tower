#include "mcv_platform.h"
#include "scared_state.h"
#include "fsm/context.h"
#include "components/player/comp_player_controller.h"
#include "components/fsm/comp_fsm.h"
#include "components/physics/controller_filter.h"
#include "components/physics/query_filter.h"

namespace FSM
{
	void ScaredState::onStart(CContext& ctx) const
	{
		CEntity* e = ctx.getOwner();
		TCompPlayerController* player = e->get<TCompPlayerController>();
        player->change_animation(player->EAnimations::NajaScare, _is_action, _delay_in, _delay_out, true);
        player->change_animation(player->EAnimations::NajaIdle, false, _delay_in, _delay_out, true);
	}

	bool ScaredState::load(const json& jData)
	{
		_is_action = jData.value("is_action", false);
		_delay_out = jData.value("delay_out", 0.01f);
		_delay_in = jData.value("delay_in", 0.01f);
		return true;
	}

	bool ScaredState::update(float dt, CContext& ctx) const
	{		
		return false;
	}
	
	void ScaredState::onFinish(CContext& ctx) const {
        ctx.setVariable("scared", false);
        ctx.setVariable("was_scared", true);
        CEntity* e = ctx.getOwner();
        TCompPlayerController* player = e->get<TCompPlayerController>();
        player->previous_state = "scared";
	}
}