#pragma once

#include "fsm/state.h"

namespace FSM
{
	class RunState : public IState
	{
		void onStart(CContext& ctx) const override;
		void onFinish(CContext& ctx);
		bool load(const json& jData);
		bool update(float dt, CContext& ctx);

	private:
		float _x_speed;
	};
}