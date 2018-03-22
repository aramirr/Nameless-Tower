#pragma once

#include "fsm/state.h"

namespace FSM
{
	class DashState : public IState
	{
		virtual void onStart(CContext& ctx) const override;
		virtual bool load(const json& jData);
		virtual bool update(float dt, CContext& ctx);

	private:
		float _dashing_max;
		float _dashing_speed;
	};

}