#include "mcv_platform.h"
#include "idle_state.h"
#include "fsm/context.h"
#include "components/player/comp_player_controller.h"
#include "components/fsm/comp_fsm.h"


namespace FSM
{
	void IdleState::onStart(CContext& ctx) const
	{
		CEntity* e = ctx.getOwner();
		TCompPlayerController* player = e->get<TCompPlayerController>();
		player->change_mesh(player->EAnimations::EIdle);		
	}

	bool IdleState::load(const json& jData)
	{
		return true;
	}

	bool IdleState::update(float dt, CContext& ctx) const
	{
		CEntity* e = ctx.getOwner();
		TCompPlayerController* player = e->get<TCompPlayerController>();
		TCompCollider* comp_collider = e->get<TCompCollider>();
		TCompTransform *c_my_transform = e->get<TCompTransform>();
		float y_speed = (player->y_speed_factor * dt) - (player->gravity * dt * dt / 2);
		if (!player->is_grounded)
			player->y_speed_factor -= player->gravity * dt / 2;

		
		if (comp_collider && comp_collider->controller)
		{
			physx::PxControllerCollisionFlags flags = comp_collider->controller->move(physx::PxVec3(0, y_speed, 0), 0.f, dt, physx::PxControllerFilters());
			if (flags.isSet(physx::PxControllerCollisionFlag::eCOLLISION_DOWN) && !player->is_grounded) {
				if (player->jumping_start_height - c_my_transform->getPosition().y > player->jumping_death_height) {
					ctx.setVariable("hit", true);
				}
				player->is_grounded = true;
				ctx.setVariable("is_grounded", true);
				ctx.setVariable("can_omni", true);
				ctx.setVariable("can_dash", true);
			}
			else if (!flags.isSet(physx::PxControllerCollisionFlag::eCOLLISION_DOWN) && player->is_grounded) {
				player->is_grounded = false;
				ctx.setVariable("is_grounded", false);
				player->jumping_start_height = c_my_transform->getPosition().y;
				ctx.setVariable("is_falling", true);
			}
			else if (!flags.isSet(physx::PxControllerCollisionFlag::eCOLLISION_DOWN)) {
 				ctx.setVariable("is_falling", true);
			}
		}
		return false;
	}

	void IdleState::onFinish(CContext& ctx) const {
		ctx.setVariable("idle", false);
	}
}