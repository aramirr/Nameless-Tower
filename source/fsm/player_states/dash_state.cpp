#include "mcv_platform.h"
#include "dash_state.h"
#include "fsm/context.h"
#include "components/player/comp_player_controller.h"

namespace FSM
{
	void DashState::onStart(CContext& ctx) const
	{
		ctx.setVariable("can_dash", false);
		CEntity* e = ctx.getOwner();
		TCompPlayerController* player = e->get<TCompPlayerController>();
		player->change_mesh(4);
	}

	bool DashState::load(const json& jData)
	{
		_x_speed = jData.value("x_speed", 10.f);
		return true;
	}

	bool DashState::update(float dt, CContext& ctx) const
	{
		CEntity* e = ctx.getOwner();
		TCompPlayerController* player = e->get<TCompPlayerController>();
		if (player->looking_left)
			player->move_player(false, false, dt, 0, _x_speed);
		else
			player->move_player(true, false, dt, 0, _x_speed);
		return false;
	}

	void DashState::onFinish(CContext& ctx) const {
		ctx.setVariable("dash", false);
	}
}