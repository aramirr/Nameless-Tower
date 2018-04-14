#include "mcv_platform.h"
#include "falling_state.h"
#include "fsm/context.h"
#include "components/player/comp_player_controller.h"

namespace FSM
{
	void FallingState::onStart(CContext& ctx) const
	{		
		CEntity* e = ctx.getOwner();
		TCompPlayerController* player = e->get<TCompPlayerController>();
		player->change_mesh(2);
	}

	bool FallingState::load(const json& jData)
	{
		_x_speed = jData.value("x_speed", 2.f);
		return true;
	}

	bool FallingState::update(float dt, CContext& ctx) const
	{
		CEntity* e = ctx.getOwner();
		TCompPlayerController* player = e->get<TCompPlayerController>();
		TCompCollider* comp_collider = e->get<TCompCollider>();
		TCompTransform *c_my_transform = e->get<TCompTransform>();
		assert(c_my_transform);
		VEC3 my_pos = c_my_transform->getPosition();
		VEC3 new_pos = my_pos;
		float y_speed;		
		y_speed = (player->y_speed_factor * dt) - (player->gravity * dt * dt * 3);
		player->y_speed_factor -= player->gravity * dt / 2;
		new_pos.y += y_speed;

		if (EngineInput["left"].isPressed()) {
			if (!player->looking_left) {
				player->looking_left = true;
				player->move_player(false, true, dt, y_speed, _x_speed);
			}
			else {
				player->move_player(false, false, dt, y_speed, _x_speed);
			}
		}
		else if (EngineInput["right"].isPressed()) {
			if (!player->looking_left) {
				player->move_player(true, false, dt, y_speed, _x_speed);
			}
			else {
				player->looking_left = false;
				player->move_player(true, true, dt, y_speed, _x_speed);
			}
		}


		VEC3 delta_move = new_pos - my_pos;
		physx::PxControllerCollisionFlags flags = comp_collider->controller->move(physx::PxVec3(delta_move.x, delta_move.y, delta_move.z), 0.f, dt, physx::PxControllerFilters());	
		if (flags.isSet(physx::PxControllerCollisionFlag::eCOLLISION_DOWN)) {
			ctx.setVariable("is_grounded", true);
		}
		return false;
	}

	void FallingState::onFinish(CContext& ctx) const {
		ctx.setVariable("is_falling", false);
	}

}

