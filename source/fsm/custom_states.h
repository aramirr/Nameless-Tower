#pragma once

#include "state.h"

namespace FSM
{
  class AnimationState : public IState
  {
    virtual void onStart(CContext& ctx) const override;
    virtual bool load(const json& jData);

  private:
    std::string _animationName;
  };
}
