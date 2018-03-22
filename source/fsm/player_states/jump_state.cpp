#include "mcv_platform.h"
#include "jump_state.h"
#include "fsm/context.h"

namespace FSM
{
	void JumpState::onStart(CContext& ctx) const
	{
		// 
	}

	bool JumpState::load(const json& jData)
	{
		_y_speed = jData.value("y_speed", 12.f);

		return true;
	}

	bool update(float dt, CContext& ctx)
	{
		return true;
	}
}