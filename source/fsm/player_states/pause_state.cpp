#include "mcv_platform.h"
#include "pause_state.h"
#include "fsm/context.h"
#include "entity/entity_parser.h"
#include "components/player/comp_player_controller.h"
#include "components/physics/controller_filter.h"
#include "components/physics/query_filter.h"

namespace FSM
{
	void PauseState::onStart(CContext& ctx) const
	{			
	}
	
	bool PauseState::load(const json& jData)
	{		
		return true;
	}

	bool PauseState::update(float dt, CContext& ctx) const
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

	void PauseState::onFinish(CContext& ctx) const {
		ctx.setVariable("pause", false);
        CEntity* e = ctx.getOwner();
        TCompPlayerController* player = e->get<TCompPlayerController>();
        player->previous_state = "pause";
	}
}

