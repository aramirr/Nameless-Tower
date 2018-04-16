#include "mcv_platform.h"
#include "pause_state.h"
#include "fsm/context.h"
#include "entity/entity_parser.h"
#include "components/player/comp_player_controller.h"

namespace FSM
{
	void PauseState::onStart(CContext& ctx) const
	{			
	}
	
	bool PauseState::load(const json& jData)
	{		
		return true;
	}

	bool PauseState::update(float dt, CContext& ctx) const
	{		
		if (EngineInput["pause"].getsPressed()) {
			ctx.setVariable("idle", true);
		}
		return false;
	}

	void PauseState::onFinish(CContext& ctx) const {
		ctx.setVariable("pause", false);
	}
}

