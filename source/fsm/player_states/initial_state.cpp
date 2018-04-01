#include "mcv_platform.h"
#include "initial_state.h"
#include "fsm/context.h"

namespace FSM
{
	void InitialState::onStart(CContext& ctx)
	{
		ctx.setVariable("center", VEC3(0.f, 0.f, 0.f));
		/*TCompTransform* my_pos = get<TCompTransform>();
		TCompCollider* comp_collider = get<TCompCollider>();
		if (comp_collider && comp_collider->controller) {
			VEC3 checkpoint = ctx.getVariable("checkpoint")->getVec();
			if (checkpoint.x)
				comp_collider->controller->setPosition(physx::PxExtendedVec3(checkpoint.x, checkpoint.y, checkpoint.z));
			else
				ctx.setVariable("checkpoint", my_pos->getPosition());
		}

		VEC3 center = ctx.getVariable("center")->getVec();
		my_pos->lookAt(my_pos->getPosition(), center);
		float y, p, r;
		my_pos->getYawPitchRoll(&y, &p, &r);
		y -= deg2rad(90);
		my_pos->setYawPitchRoll(y, 0, 0);

		ctx.setVariable("lookingLeft", my_pos->isInLeft(center) ? false : true);*/
		//change_mesh(1);
		//ChangeState("idle");
	}

	bool InitialState::load(const json& jData)
	{		
		return true;
	}

	bool InitialState::update(float dt, CContext& ctx)
	{
		return true;
	}

	void InitialState::onFinish(CContext& ctx) {

	}
}

