#pragma once

#include "fsm/state.h"

namespace FSM
{
	class DeadState : public IState
	{
		virtual void onStart(CContext& ctx) const override;
		virtual bool load(const json& jData);
		virtual bool update(float dt, CContext& ctx);
	};
}