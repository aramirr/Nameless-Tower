#include "mcv_platform.h"
#include "falling_omni_state.h"
#include "fsm/context.h"
#include "components/player/comp_player_controller.h"
#include "components/fsm/comp_fsm.h"
#include "components/physics/controller_filter.h"
#include "components/physics/query_filter.h"

namespace FSM
{
	void FallingOmniState::onStart(CContext& ctx) const
	{		
		CEntity* e = ctx.getOwner();
		TCompPlayerController* player = e->get<TCompPlayerController>();
		player->change_animation(player->EAnimations::NajaJumpFall, _is_action, _delay_in, _delay_out);
		if (player->omnidash_arrow.y > 0.9)
			player->y_speed_factor = player->omnidash_arrow.y * 6;
		else if (player->omnidash_arrow.y > 0.6)
			player->y_speed_factor = player->omnidash_arrow.y * 11;
		else if (player->omnidash_arrow.y > 0.3)
			player->y_speed_factor = player->omnidash_arrow.y * 13;
		else
			player->y_speed_factor = player->omnidash_arrow.y * 14;
	}

	bool FallingOmniState::load(const json& jData)
	{
		_x_speed = jData.value("x_speed", 2.f);
		_is_action = jData.value("is_action", false);
		_delay_out = jData.value("delay_out", 0.01f);
		_delay_in = jData.value("delay_in", 0.01f);
		return true;
	}

	bool FallingOmniState::update(float dt, CContext& ctx) const
	{
		CEntity* e = ctx.getOwner();
		TCompPlayerController* player = e->get<TCompPlayerController>();
		TCompCollider* comp_collider = e->get<TCompCollider>();
		TCompTransform *c_my_transform = e->get<TCompTransform>();
		assert(c_my_transform);
		VEC3 my_pos = c_my_transform->getPosition();
		VEC3 new_pos = my_pos + (player->omnidash_vector * dt);;
		float y_speed;		
		y_speed = (player->y_speed_factor * dt) - (player->gravity * dt * dt * 2);
		if (player->y_speed_factor > -12)
			player->y_speed_factor -= player->gravity * dt / 3;
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
		else {
			

			PxShape* player_shape;
			comp_collider->controller->getActor()->getShapes(&player_shape, 1);
			PxFilterData filter_data = player_shape->getSimulationFilterData();
			ControllerFilterCallback *filter_controller = new ControllerFilterCallback();
			BasicQueryFilterCallback *query_filter = new BasicQueryFilterCallback();

			player->omnidash_vector = c_my_transform->getFront();
			if (player->looking_left)
				player->omnidash_vector *= player->omnidash_arrow.x * _x_speed * 3.5;
			else
				player->omnidash_vector *= player->omnidash_arrow.x * _x_speed * 3.5 * -1;

			new_pos = my_pos + (player->omnidash_vector * dt);
			y_speed = (player->y_speed_factor * dt) - (player->gravity * dt * dt * 3);
			new_pos.y += y_speed;
			VEC3 delta_move = new_pos - my_pos;			

			float current_yaw;
			float current_pitch;
			float amount_moved = _x_speed * dt;
			c_my_transform->getYawPitchRoll(&current_yaw, &current_pitch);
			current_yaw = current_yaw - (0.1f * player->omnidash_arrow.x * amount_moved);
			c_my_transform->setYawPitchRoll(current_yaw, current_pitch);
			PxControllerCollisionFlags flags = comp_collider->controller->move(PxVec3(delta_move.x, delta_move.y, delta_move.z), 0.f, dt, PxControllerFilters(&filter_data, query_filter, filter_controller));

			if (flags.isSet(physx::PxControllerCollisionFlag::eCOLLISION_DOWN)) {
				if (player->jumping_start_height - c_my_transform->getPosition().y > player->jumping_death_height) {
					ctx.setVariable("hit", true);
				}
				ctx.setVariable("is_grounded", true);
				ctx.setVariable("can_omni", true);
				ctx.setVariable("can_dash", true);
			}
			if (flags.isSet(physx::PxControllerCollisionFlag::eCOLLISION_SIDES) || flags.isSet(physx::PxControllerCollisionFlag::eCOLLISION_UP)) {				
				ctx.setVariable("is_falling", true);
			}
		}		
		return false;
	}

	void FallingOmniState::onFinish(CContext& ctx) const {
		ctx.setVariable("is_falling", false);
	}

}

