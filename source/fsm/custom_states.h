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

  class JumpState : public IState
  {
    virtual void onStart(CContext& ctx) const override;
    virtual bool load(const json& jData);

  private:
    float _y_speed;
  };

	// PLAYER STATES
	class IdleState : public IState
	{
		virtual void onStart(CContext& ctx) const override;
		virtual bool load(const json& jData);
	};

	class RunState : public IState
	{
		virtual void onStart(CContext& ctx) const override;
		virtual bool load(const json& jData);

	private:
		float _x_speed;
	};

	class DashState : public IState
	{
		virtual void onStart(CContext& ctx) const override;
		virtual bool load(const json& jData);

	private:
		float _dashing_max;
		float _dashing_speed;
	};

	class OmnidashState : public IState
	{
		virtual void onStart(CContext& ctx) const override;
		virtual bool load(const json& jData);

	private:
		float _omnidash_max_time;
	};

	class OmnijumpState : public IState
	{
		virtual void onStart(CContext& ctx) const override;
		virtual bool load(const json& jData);

	private:
		float _omnidashing_max_amount;
	};

	class DeadState : public IState
	{
		virtual void onStart(CContext& ctx) const override;
		virtual bool load(const json& jData);
	};
}
