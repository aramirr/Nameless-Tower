#pragma once

#include "fsm/state.h"

namespace FSM
{
	class InitialState : public IState
	{
		void onStart(CContext& ctx);
		void onFinish(CContext& ctx);
		bool load(const json& jData);
		bool update(float dt, CContext& ctx);

		DECL_SIBLING_ACCESS();

	private:
	};

}