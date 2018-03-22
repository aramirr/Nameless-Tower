#include "mcv_platform.h"
#include "omnijump_state.h"
#include "fsm/context.h"

namespace FSM
{
	void OmnijumpState::onStart(CContext& ctx) const
	{
		// ..
	}

	bool OmnijumpState::load(const json& jData)
	{
		_omnidashing_max_amount = jData.value("omnidashing_max_amount", 1.3f);

		return true;
	}

	bool OmnijumpState::update(float dt, CContext& ctx)
	{
		return true;
	}
}