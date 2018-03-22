#include "mcv_platform.h"
#include "omnidash_state.h"
#include "fsm/context.h"

namespace FSM
{
	void OmnidashState::onStart(CContext& ctx) const
	{
		// ..
	}

	bool OmnidashState::load(const json& jData)
	{
		_omnidash_max_time = jData.value("omnidash_max_time", 0.3f);

		return true;
	}

	bool OmnidashState::update(float dt, CContext& ctx)
	{
		return true;
	}
}