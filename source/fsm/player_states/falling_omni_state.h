#pragma once

#include "fsm/state.h"
#include "fsm/player_states/player_state.h"

namespace FSM
{
	class FallingOmniState : public PlayerState
	{
		void onStart(CContext& ctx) const override;
		bool load(const json& jData) override;
		bool update(float dt, CContext& ctx) const override;
		void onFinish(CContext& ctx) const override;

	private:
		float _x_speed;
	};
}