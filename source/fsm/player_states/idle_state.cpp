#include "mcv_platform.h"
#include "idle_state.h"
#include "fsm/context.h"

namespace FSM
{
	void IdleState::onStart(CContext& ctx) const
	{
		// ..
	}

	bool IdleState::load(const json& jData)
	{
		// ..

		return true;
	}

	bool IdleState::update(float dt, CContext& ctx)
	{
		return true;
	}
}