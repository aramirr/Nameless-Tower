#pragma once

#include "fsm/state.h"

namespace FSM
{
	class RunState : public IState
	{
		void onStart(CContext& ctx) const override;
		bool load(const json& jData) override;
		bool update(float dt, CContext& ctx) const override;
		void onFinish(CContext& ctx) const override;

	private:
		float _x_speed;
	};
}