#pragma once

#include "variable.h"
#include "fsm.h"

namespace FSM
{
  class CMachine;
  class IState;

  class CContext
  {
  public:
    enum class EState { IDLE = 0, RUNNING, FINISHED };

    void setFSM(const CMachine* fsm);
    void start();
    void stop();
    void update(float delta);

    void setCurrentState(const IState* state);

    const CVariable* getVariable(const std::string& name) const;
    void getStateTransitions(const IState* state, VTransitions& output) const;

  private:
    const CMachine* _fsm = nullptr;
    EState _state = EState::IDLE;
    const IState* _currentState = nullptr;
    VVariables _variables;
  };
}
