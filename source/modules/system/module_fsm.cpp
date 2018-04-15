#include "mcv_platform.h"
#include "module_fsm.h"

#include "fsm/fsm.h"
#include "fsm/custom_states.h"
#include "fsm/custom_transitions.h"

CModuleFSM::CModuleFSM(const std::string& name)
	: IModule(name)
{}

bool CModuleFSM::start()
{
  Resources.registerResourceClass(getResourceClassOf<FSM::CMachine>());

  _factory.registerInstancer("AnimationState", new FSM::StateInstancer<FSM::AnimationState>());
  _factory.registerInstancer("JumpState", new FSM::StateInstancer<FSM::JumpState>());
  _factory.registerInstancer("HitState", new FSM::StateInstancer<FSM::HitState>());
  
  _factory.registerInstancer("VariableTransition", new FSM::TransitionInstancer<FSM::VariableTransition>());
  _factory.registerInstancer("TimeTransition", new FSM::TransitionInstancer<FSM::TimeTransition>());

  return true;
}

bool CModuleFSM::stop()
{
  return true;
}
