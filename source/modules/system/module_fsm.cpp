#include "mcv_platform.h"
#include "module_fsm.h"

#include "fsm/fsm.h"
#include "fsm/custom_states.h"
#include "fsm/player_states/jump_state.h"
#include "fsm/player_states/dash_state.h"
#include "fsm/player_states/dead_state.h"
#include "fsm/player_states/initial_state.h"
#include "fsm/player_states/omnidash_state.h"
#include "fsm/player_states/omnijump_state.h"
#include "fsm/player_states/idle_state.h"
#include "fsm/player_states/run_state.h"
#include "fsm/player_states/falling_state.h"
#include "fsm/player_states/pause_state.h"
#include "fsm/player_states/windstrike_state.h"
#include "fsm/player_states/glide_state.h"
#include "fsm/player_states/landing_state.h"
#include "fsm/player_states/dying_state.h"
#include "fsm/player_states/falling_omni_state.h"
#include "fsm/player_states/falling_dash_state.h"
#include "fsm/player_states/turn_around_state.h"
#include "fsm/player_states/look_up_state.h"
#include "fsm/player_states/look_down_state.h"
#include "fsm/custom_transitions.h"

CModuleFSM::CModuleFSM(const std::string& name)
	: IModule(name)
{}

bool CModuleFSM::start()
{
  Resources.registerResourceClass(getResourceClassOf<FSM::CMachine>());

	_factory.registerInstancer("InitialState", new FSM::StateInstancer<FSM::InitialState>());
	_factory.registerInstancer("JumpState", new FSM::StateInstancer<FSM::JumpState>());
	_factory.registerInstancer("IdleState", new FSM::StateInstancer<FSM::IdleState>());
	_factory.registerInstancer("RunState", new FSM::StateInstancer<FSM::RunState>());
	_factory.registerInstancer("DashState", new FSM::StateInstancer<FSM::DashState>());
	_factory.registerInstancer("OmnidashState", new FSM::StateInstancer<FSM::OmnidashState>());
	_factory.registerInstancer("OmnijumpState", new FSM::StateInstancer<FSM::OmnijumpState>());
	_factory.registerInstancer("DeadState", new FSM::StateInstancer<FSM::DeadState>());
	_factory.registerInstancer("FallingState", new FSM::StateInstancer<FSM::FallingState>());
	_factory.registerInstancer("WindstrikeState", new FSM::StateInstancer<FSM::WindstrikeState>());
	_factory.registerInstancer("PauseState", new FSM::StateInstancer<FSM::PauseState>());
	_factory.registerInstancer("GlideState", new FSM::StateInstancer<FSM::GlideState>());
	_factory.registerInstancer("LandingState", new FSM::StateInstancer<FSM::LandingState>());
	_factory.registerInstancer("DyingState", new FSM::StateInstancer<FSM::DyingState>());
	_factory.registerInstancer("FallingOmniState", new FSM::StateInstancer<FSM::FallingOmniState>());
	_factory.registerInstancer("FallingDashState", new FSM::StateInstancer<FSM::FallingDashState>());
    _factory.registerInstancer("TurnAroundState", new FSM::StateInstancer<FSM::TurnAroundState>());
    _factory.registerInstancer("LookUpState", new FSM::StateInstancer<FSM::LookUpState>());
    _factory.registerInstancer("LookDownState", new FSM::StateInstancer<FSM::LookDownState>());
  
	_factory.registerInstancer("VariableTransition", new FSM::TransitionInstancer<FSM::VariableTransition>());
	_factory.registerInstancer("LogicTransition", new FSM::TransitionInstancer<FSM::LogicTransition>());
  _factory.registerInstancer("TimeTransition", new FSM::TransitionInstancer<FSM::TimeTransition>());

  return true;
}

bool CModuleFSM::stop()
{
  return true;
}
