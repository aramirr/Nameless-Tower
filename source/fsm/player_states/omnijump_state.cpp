#include "mcv_platform.h"
#include "omnijump_state.h"
#include "components/player/comp_player_controller.h"
#include "fsm/context.h"
#include "components/physics/controller_filter.h"

namespace FSM
{
	void OmnijumpState::onStart(CContext& ctx) const
	{
		CEntity* e = ctx.getOwner();
		TCompPlayerController* player = e->get<TCompPlayerController>();
		player->change_animation(player->EAnimations::NajaOmniAr, _is_action, _delay_in, _delay_out);
		player->y_speed_factor = _y_speed;
	}

	bool OmnijumpState::load(const json& jData)
	{
		_y_speed = jData.value("y_speed", 12.f);
		_x_speed = jData.value("x_speed", 2.f);
		_is_action = jData.value("is_action", false);
		_delay_out = jData.value("delay_out", 0.01f);
		_delay_in = jData.value("delay_out", 0.01f);
		return true;
	}

	bool OmnijumpState::update(float dt, CContext& ctx) const
	{
		CEntity* e = ctx.getOwner();
		TCompPlayerController* player = e->get<TCompPlayerController>();

		TCompCollider* comp_collider = e->get<TCompCollider>();
		TCompTransform *c_my_transform = e->get<TCompTransform>();
		VEC3 my_pos = c_my_transform->getPosition();
		player->omnidash_vector = c_my_transform->getFront();
		if (player->looking_left)
			player->omnidash_vector *= player->omnidash_arrow.x;
		else
			player->omnidash_vector *= player->omnidash_arrow.x * -1;

		player->omnidash_vector.y += player->omnidash_arrow.y;
		VEC3 new_pos;
		new_pos = my_pos + (player->omnidash_vector * ((_y_speed * 6 - player->gravity * dt) * dt));
		player->y_speed_factor -= player->gravity * dt / 4;

		VEC3 centre = VEC3(0, new_pos.y, 0);
		float d = VEC3::Distance(centre, new_pos);
		d = (player->tower_radius) / d;
		new_pos.x = new_pos.x * d;
		new_pos.z = new_pos.z * d;

		VEC3 delta_move = new_pos - my_pos;		

		float current_yaw;
		float current_pitch;
		float amount_moved = _x_speed * dt;
		c_my_transform->getYawPitchRoll(&current_yaw, &current_pitch);
		current_yaw = current_yaw - (0.4f * player->omnidash_arrow.x * amount_moved);
		c_my_transform->setYawPitchRoll(current_yaw, current_pitch);

		PxShape* player_shape;
		comp_collider->controller->getActor()->getShapes(&player_shape, 1);
		PxFilterData filter_data = player_shape->getSimulationFilterData();
		ControllerFilterCallback *filter_controller = new ControllerFilterCallback();
		PxControllerCollisionFlags flags = comp_collider->controller->move(PxVec3(delta_move.x, delta_move.y, delta_move.z), 0.f, dt, PxControllerFilters(&filter_data, filter_controller, filter_controller));

		if (flags.isSet(physx::PxControllerCollisionFlag::eCOLLISION_UP) || flags.isSet(physx::PxControllerCollisionFlag::eCOLLISION_SIDES) || flags.isSet(physx::PxControllerCollisionFlag::eCOLLISION_DOWN)) {
			ctx.setVariable("idle", true);
		}

		return false;
	}

	void OmnijumpState::onFinish(CContext& ctx) const {
		ctx.setVariable("omnijump", false);
		CEntity* e = ctx.getOwner();
		TCompPlayerController* player = e->get<TCompPlayerController>();
		player->y_speed_factor = 0;
	}
}