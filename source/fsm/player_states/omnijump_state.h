#pragma once

#include "fsm/state.h"

namespace FSM
{
	class OmnijumpState : public IState
	{
		virtual void onStart(CContext& ctx) const override;
		virtual bool load(const json& jData);
		virtual bool update(float dt, CContext& ctx);

	private:
		float _omnidashing_max_amount;
	};
}