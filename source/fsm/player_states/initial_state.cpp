#include "mcv_platform.h"
#include "initial_state.h"
#include "fsm/context.h"
#include "components/player/comp_player_controller.h"

namespace FSM
{
	void InitialState::onStart(CContext& ctx) const
	{
		CEntity* e = ctx.getOwner();
		TCompPlayerController* player = e->get<TCompPlayerController>();
		TCompTransform* my_pos = e->get<TCompTransform>();
		TCompCollider* comp_collider = e->get<TCompCollider>();
		VEC3 position;
		if (comp_collider && comp_collider->controller) {
			if (player->checkpoint.x) {
				comp_collider->controller->setPosition(physx::PxExtendedVec3(player->checkpoint.x, player->checkpoint.y, player->checkpoint.z));
				position = VEC3(player->checkpoint.x, player->checkpoint.y, player->checkpoint.z);
			}				
			else {
				player->checkpoint = my_pos->getPosition();
				position = my_pos->getPosition();
			}				
		}
		else {
			player->checkpoint = my_pos->getPosition();
			position = my_pos->getPosition();
		}

		my_pos->lookAt(position, player->center);
		float y, p, r;
		my_pos->getYawPitchRoll(&y, &p, &r);
		y -= deg2rad(90);
		my_pos->setYawPitchRoll(y, 0, 0);

		player->looking_left = my_pos->isInLeft(player->center) ? false : true;		
	}

	bool InitialState::load(const json& jData)
	{		
		return true;
	}

	bool InitialState::update(float dt, CContext& ctx) const
	{
		ctx.setVariable("is_grounded", true);
		return false;
	}

	void InitialState::onFinish(CContext& ctx) const {
		ctx.setVariable("initial", false);
	}
}

