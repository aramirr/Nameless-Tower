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

  void IdleState::onStart(CContext& ctx) const
  {
    // ..
  }

  bool IdleState::load(const json& jData)
  {
    // ..

    return true;
  }
	
	void RunState::onStart(CContext& ctx) const
	{
		// ..
	}
	
	bool RunState::load(const json& jData)
	{
		_x_speed = jData.value("x_speed", 2.f);

		return true;
	}

	void JumpState::onStart(CContext& ctx) const
	{
		// 
	}

	bool JumpState::load(const json& jData)
	{
		_y_speed = jData.value("y_speed", 12.f);

		return true;
	}

	void DashState::onStart(CContext& ctx) const
	{
		// ..
	}
	
	bool DashState::load(const json& jData)
	{
		_dashing_max = jData.value("dashing_max", 0.35f);
		_dashing_speed = jData.value("dashing_speed", 3.f);

		return true;
	}

	void OmnidashState::onStart(CContext& ctx) const
	{
		// ..
	}
	
	bool OmnidashState::load(const json& jData)
	{
		_omnidash_max_time = jData.value("omnidash_max_time", 0.3f);

		return true;
	}

	void OmnijumpState::onStart(CContext& ctx) const
	{
		// ..
	}
	
	bool OmnijumpState::load(const json& jData)
	{
		_omnidashing_max_amount = jData.value("omnidashing_max_amount", 1.3f);

		return true;
	}

	void DeadState::onStart(CContext& ctx) const
	{
		// ..
	}
	
	bool DeadState::load(const json& jData)
	{
		return true;
	}

}