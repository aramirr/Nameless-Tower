#include "mcv_platform.h"
#include "jump_state.h"
#include "fsm/context.h"
#include "components/player/comp_player_controller.h"

namespace FSM
{
	void JumpState::onStart(CContext& ctx) const
	{
	}

	bool JumpState::load(const json& jData)
	{
		_y_speed = jData.value("y_speed", 12.f);
		return true;
	}

	bool JumpState::update(float dt, CContext& ctx) const
	{
		CEntity* e = ctx.getOwner();
		TCompPlayerController* player = e->get<TCompPlayerController>();
		TCompCollider* comp_collider = e->get<TCompCollider>();
		TCompTransform *c_my_transform = e->get<TCompTransform>();
		assert(c_my_transform);
		VEC3 my_pos = c_my_transform->getPosition();
		VEC3 new_pos = my_pos;
		float y_speed;
		if (player->y_speed_factor > 0)
			y_speed = (player->y_speed_factor * dt) - (player->gravity * dt * dt / 2);
		else
			y_speed = (player->y_speed_factor * dt) - (player->gravity * dt * dt * 3);
		player->y_speed_factor -= player->gravity * dt / 2;
		new_pos.y += y_speed;


		/*// Chequea movimiento
		if (isPressed('A')) {
			if (!looking_left) {
				looking_left = true;
				move_player(false, true, DT, y_speed);
			}
			else {
				move_player(false, false, DT, y_speed);
			}
		}
		else if (isPressed('D')) {
			if (!looking_left) {
				move_player(true, false, DT, y_speed);
			}
			else {
				looking_left = false;
				move_player(true, true, DT, y_speed);
			}
		}
		else {*/
		VEC3 delta_move = new_pos - my_pos;
		physx::PxControllerCollisionFlags flags = comp_collider->controller->move(physx::PxVec3(delta_move.x, delta_move.y, delta_move.z), 0.f, DT, physx::PxControllerFilters());
		if (flags.isSet(physx::PxControllerCollisionFlag::eCOLLISION_UP)) {
			change_mesh(1);
			y_speed_factor = 0;
			ChangeState("idle");
		}
		else if (flags.isSet(physx::PxControllerCollisionFlag::eCOLLISION_DOWN)) {
			change_mesh(1);
			y_speed_factor = 0;
			ChangeState("idle");
		}
		//}
		return false;
	}
}