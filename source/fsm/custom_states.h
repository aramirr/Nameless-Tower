#pragma once

#include "state.h"
#include "components/juan/comp_transform.h"
#include "components/camera/comp_camera.h"
#include "components/ui/ui_mouse_pos.h"

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
