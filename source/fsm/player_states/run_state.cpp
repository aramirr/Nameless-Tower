#include "mcv_platform.h"
#include "run_state.h"
#include "fsm/context.h"

namespace FSM
{
	void RunState::onStart(CContext& ctx) const
	{
		// ..
	}

	bool RunState::load(const json& jData)
	{
		_x_speed = jData.value("x_speed", 2.f);

		return true;
	}

	bool RunState::update(float dt, CContext& ctx)
	{
		return true;
	}
}