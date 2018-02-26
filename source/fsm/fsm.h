#pragma once

#include "state.h"
#include "transition.h"

namespace FSM
{
  class CMachine
  {
  public:

    void load(const json& jData);

    const IState* getInitialState() const { return _initialState; }
    const VStates& getStates() const { return _states; }
    const VTransitions& getTransitions() const { return _transitions; }

  private:
    VStates _states;
    VTransitions _transitions;
    const IState* _initialState = nullptr;
  };
}
