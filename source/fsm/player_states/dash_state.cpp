#include "mcv_platform.h"
#include "dash_state.h"
#include "fsm/context.h"

namespace FSM
{
	void DashState::onStart(CContext& ctx) const
	{
		// 
	}

	bool DashState::load(const json& jData)
	{
		return true;
	}

	bool DashState::update(float dt, CContext& ctx)
	{
		return true;
	}
}