#include "mcv_platform.h"
#include "dead_state.h"
#include "fsm/context.h"

namespace FSM
{
	void DeadState::onStart(CContext& ctx) const
	{
		// ..
	}

	bool DeadState::load(const json& jData)
	{
		return true;
	}

	bool DeadState::update(float dt, CContext& ctx)
	{
		return true;
	}
}