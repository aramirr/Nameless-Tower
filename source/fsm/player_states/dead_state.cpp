#include "mcv_platform.h"
#include "dead_state.h"
#include "fsm/context.h"
#include "components/player/comp_player_controller.h"
#include "components/fsm/comp_fsm.h"
#include "components/physics/controller_filter.h"
#include "components/physics/query_filter.h"

namespace FSM
{
	void DeadState::onStart(CContext& ctx) const
	{
		CEntity* e = ctx.getOwner();
		TCompPlayerController* player = e->get<TCompPlayerController>();
    player->change_animation(player->EAnimations::NajaDeath, _is_action, _delay_in, _delay_out, true);
		ctx.setVariable("hit", false);
		_sound->setVolume(player->volumen);
        
	}

	bool DeadState::load(const json& jData)
	{
		_is_action = jData.value("is_action", false);
		_delay_out = jData.value("delay_out", 0.01f);
		_delay_in = jData.value("delay_in", 0.01f);        
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
			BasicQueryFilterCallback *query_filter = new BasicQueryFilterCallback();
			PxControllerCollisionFlags flags = comp_collider->controller->move(PxVec3(0, y_speed, 0), 0.f, dt, PxControllerFilters(&filter_data, query_filter, filter_controller));
			
		}
		return false;
	}
	
	void DeadState::onFinish(CContext& ctx) const {
		ctx.setVariable("dead", false);			
        CEntity* e = ctx.getOwner();
        TCompPlayerController* player = e->get<TCompPlayerController>();
        player->previous_state = "dead";
	}
}