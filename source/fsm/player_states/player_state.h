#pragma once

#include "fsm/state.h"
#include <string>

namespace FSM
{
	class PlayerState : public IState
	{

	protected:
		float _delay_in;
		float _delay_out;
		bool  _is_action;
        Studio::EventInstance*  _sound;
        Studio::EventInstance*  _sound_cloth_rustle;
	};

}