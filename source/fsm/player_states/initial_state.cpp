#include "mcv_platform.h"
#include "initial_state.h"
#include "fsm/context.h"

namespace FSM
{
	void InitialState::onStart(CContext& ctx) const
	{
		// 
	}

	bool InitialState::load(const json& jData)
	{
		return true;
	}

	bool InitialState::update(float dt, CContext& ctx)
	{
		TCompTransform *my_pos = getMyTransform();
		TCompCollider* comp_collider = get<TCompCollider>();
		if (comp_collider && comp_collider->controller) {
			if (checkpoint.x)
				comp_collider->controller->setPosition(physx::PxExtendedVec3(checkpoint.x, checkpoint.y, checkpoint.z));
			else
				checkpoint = my_pos->getPosition();
		}


		my_pos->lookAt(my_pos->getPosition(), center);
		float y, p, r;
		my_pos->getYawPitchRoll(&y, &p, &r);
		y -= deg2rad(90);
		my_pos->setYawPitchRoll(y, 0, 0);

		looking_left = my_pos->isInLeft(center) ? false : true;
		change_mesh(1);
		ChangeState("idle");
		return true;
	}
}