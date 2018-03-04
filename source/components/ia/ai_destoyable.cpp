#include "mcv_platform.h"
#include "handle/handle.h"
#include "ai_destoyable.h"
#include "entity/entity_parser.h"
#include "components/comp_transform.h"
#include "render/render_utils.h"

DECL_OBJ_MANAGER("ai_destroyable", CAIDestroyable);

using namespace physx;

void CAIDestroyable::Init()
{
	// insert all states in the map
	AddState("idle", (statehandler)&CAIDestroyable::IdleState);
	AddState("trigger_destroy", (statehandler)&CAIDestroyable::TriggerDestroyState);
	AddState("transition_destroy", (statehandler)&CAIDestroyable::TransitionDestroyState);
	AddState("destroy", (statehandler)&CAIDestroyable::DestoyState);


	// reset the state
	ChangeState("idle");
}

void CAIDestroyable::debugInMenu() {

	IAIController::debugInMenu();
	CEntity *c_trigger = (CEntity *)getEntityByName(trigger_actor);
	if (!c_trigger)
		return;

	TCompTransform *mypos = getMyTransform();
	TCompTransform *ppos = c_trigger->get<TCompTransform>();

	ImGui::Text("Distance %f", VEC3::Distance(mypos->getPosition(), ppos->getPosition()));
	ImGui::Text("acum_time %f", acum_time);


}

void CAIDestroyable::load(const json& j, TEntityParseContext& ctx) {
	setEntity(ctx.current_entity);

	Init();

	trigger_actor = j.value("trigger_actor", "");
	destroy_time = j.value("destroy_time", 1.0f);
	recover_time = j.value("recover_time", 5.0f);
	acum_time = 0;
}



void CAIDestroyable::IdleState()
{
	change_color(VEC4(0, 1, 0, 1));
	CEntity *c_trigger = (CEntity *)getEntityByName(trigger_actor);
	if (!c_trigger)
		return;

	TCompTransform *mypos = getMyTransform();
	TCompTransform *ppos = c_trigger->get<TCompTransform>();

	float distance = VEC3::Distance(mypos->getPosition(), ppos->getPosition());
	if (distance < 0.75f) {
		acum_time = 0.0f;
		ChangeState("trigger_destroy");
	}
}


void CAIDestroyable::TriggerDestroyState(float dt)
{
	change_color(VEC4(1, 0, 0, 1));
	acum_time += dt;
	if (acum_time >= destroy_time)
	{
		acum_time = 0;
		ChangeState("transition_destroy");
	}
}

void CAIDestroyable::TransitionDestroyState(float dt)
{
	change_color(VEC4(1, 1, 0, 1));
	TCompTransform *mypos = getMyTransform();
	if (!mypos) {
		return;
	}
	current_pos = mypos->getPosition();
	mypos->setPosition(VEC3(0, 0, 0));
	TCompCollider* comp_collider = get<TCompCollider>();
	if (comp_collider)
	{
		PxRigidActor* rigidActor = ((PxRigidActor*)comp_collider->actor);
		PxTransform tr = rigidActor->getGlobalPose();
		tr.p = PxVec3(0, 0, 0);
		rigidActor->setGlobalPose(tr);
	}
	ChangeState("destroy");
}

void CAIDestroyable::DestoyState(float dt)
{
	change_color(VEC4(1, 0, 1, 1));
	acum_time += dt;
	if (acum_time >= recover_time)
	{
		acum_time = 0;
		TCompTransform *mypos = getMyTransform();
		if (!mypos) {
			return;
		}

		CEntity *p_e = (CEntity *)getEntityByName(trigger_actor);
		if (!p_e)
			return;

		TCompTransform *t_ppos = p_e->get<TCompTransform>();
		if (!t_ppos) {
			return;
		}

		float distance = VEC3::Distance(current_pos, t_ppos->getPosition());
		if (distance < 1.5f) {
			return;
		}

		mypos->setPosition(current_pos);
		TCompCollider* comp_collider = get<TCompCollider>();
		if (comp_collider)
		{
			PxRigidActor* rigidActor = ((PxRigidActor*)comp_collider->actor);
			PxTransform tr = rigidActor->getGlobalPose();
			tr.p = PxVec3(current_pos.x, current_pos.y, current_pos.z);
			rigidActor->setGlobalPose(tr);
		}
		ChangeState("idle");
	}
}

void CAIDestroyable::change_color(VEC4 color) {
	TCompRender *my_render = getMyRender();
	if (my_render)
	{
		my_render->color = color;
	}
}