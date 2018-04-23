#pragma once

#include "fsm/state.h"

namespace FSM
{
	class PlayerState : public IState
	{

	protected:
		float _delay_in;
		float _delay_out;
		bool  _is_action;
	};

}