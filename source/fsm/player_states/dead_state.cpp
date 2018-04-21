#include "mcv_platform.h"
#include "dead_state.h"
#include "fsm/context.h"
#include "components/player/comp_player_controller.h"
#include "components/fsm/comp_fsm.h"
#include "components/physics/controller_filter.h"

namespace FSM
{
	void DeadState::onStart(CContext& ctx) const
	{
		CEntity* e = ctx.getOwner();
		TCompPlayerController* player = e->get<TCompPlayerController>();
		player->change_mesh(player->EAnimations::EDead);	
		ctx.setVariable("hit", false);
	}

	bool DeadState::load(const json& jData)
	{
		return true;
	}

	bool DeadState::update(float dt, CContext& ctx) const
	{
		CEntity* e = ctx.getOwner();
		TCompPlayerController* player = e->get<TCompPlayerController>();
		TCompTransform *c_my_transform = e->get<TCompTransform>();
		TCompCollider* comp_collider = e->get<TCompCollider>();
		float y_speed = (player->y_speed_factor * dt) - (player->gravity * dt * dt / 2);
		if (!player->is_grounded)
			player->y_speed_factor -= player->gravity * dt / 2;
		if (comp_collider && comp_collider->controller)
		{	
			PxShape* player_shape;
			comp_collider->controller->getActor()->getShapes(&player_shape, 1);
			PxFilterData filter_data = player_shape->getSimulationFilterData();
			ControllerFilterCallback *filter_controller = new ControllerFilterCallback();
			PxControllerCollisionFlags flags = comp_collider->controller->move(PxVec3(0, y_speed, 0), 0.f, dt, PxControllerFilters(&filter_data, filter_controller, filter_controller));

			if (flags.isSet(physx::PxControllerCollisionFlag::eCOLLISION_DOWN) && !player->is_grounded) {				
				player->y_speed_factor = 0;
				player->is_grounded = true;
				ctx.setVariable("dead", false);
				ctx.setVariable("can_dash", true);
				ctx.setVariable("can_omni", true);
				ctx.setVariable("is_grounded", true);
			}			
		}
		return false;
	}
	
	void DeadState::onFinish(CContext& ctx) const {
		ctx.setVariable("dead", false);		
		CEntity* e = ctx.getOwner();
		TCompPlayerController* player = e->get<TCompPlayerController>();		
		TCompCollider* comp_collider = e->get<TCompCollider>();
		if (comp_collider && comp_collider->controller) {
			comp_collider->controller->setPosition(physx::PxExtendedVec3(player->checkpoint.x, player->checkpoint.y, player->checkpoint.z));
		}
	}
}