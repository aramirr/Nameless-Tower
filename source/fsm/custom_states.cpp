#include "mcv_platform.h"
#include "custom_states.h"
#include "context.h"

#include "components/controllers/comp_fake_animation_controller.h"

namespace FSM
{
  void AnimationState::onStart(CContext& ctx) const
  {
    CEntity* e = ctx.getOwner();
    e->sendMsg(TMsgAnimation{ _animationName });
  }

  bool AnimationState::load(const json& jData)
  {
    _animationName = jData.value("animation", "");

    return true;
  }
}