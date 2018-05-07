#include "mcv_platform.h"
#include "handle/handle.h"
#include "ai_destoyable.h"
#include "entity/entity_parser.h"
#include "components/juan/comp_transform.h"
#include "render/render_utils.h"

DECL_OBJ_MANAGER("ai_destroyable", CAIDestroyable);

using namespace physx;

void CAIDestroyable::debugInMenu() {
	IAIController::debugInMenu();
	ImGui::Text("acum_time %f", acum_time);
}

void CAIDestroyable::registerMsgs() {
	DECL_MSG(CAIDestroyable, TMsgDestroy, onTriggerEnter);
}

void CAIDestroyable::onTriggerEnter(const TMsgDestroy& msg) {
	acum_time = 0.0f;
	ChangeState("trigger_destroy");
};

void CAIDestroyable::load(const json& j, TEntityParseContext& ctx) {
	setEntity(ctx.current_entity);
	
	destroy_time = j.value("destroy_time", 1.0f);
	recover_time = j.value("recover_time", 5.0f);
	acum_time = 0;
	Init();
}

void CAIDestroyable::Init()
{
	// insert all states in the map
	AddState("idle", (statehandler)&CAIDestroyable::IdleState);
	AddState("trigger_destroy", (statehandler)&CAIDestroyable::TriggerDestroyState);
	AddState("transition_destroy", (statehandler)&CAIDestroyable::TransitionDestroyState);
	AddState("destroy", (statehandler)&CAIDestroyable::DestroyState);

	// reset the state
	ChangeState("idle");
}

void CAIDestroyable::IdleState()
{
	change_mesh(0);
}


void CAIDestroyable::TriggerDestroyState(float dt)
{
	change_mesh(0);
	acum_time += DT;
	if (acum_time >= destroy_time)
	{
		acum_time = 0;
		ChangeState("transition_destroy");
	}
}

void CAIDestroyable::TransitionDestroyState(float dt)
{
	
	TCompTransform *mypos = getMyTransform();
	if (!mypos) {
		return;
	}
	current_pos = mypos->getPosition();
	change_mesh(1);
	TCompCollider* comp_collider = get<TCompCollider>();
	if (comp_collider)
	{
		comp_collider->config.group = EnginePhysics.getFilterByName("windstrike");
		comp_collider->config.mask = -4;
		PxRigidActor* rigidActor = ((PxRigidActor*)comp_collider->actor);
		PxTransform tr = rigidActor->getGlobalPose();
		tr.p = PxVec3(0, 0, 0);
		rigidActor->setGlobalPose(tr);
	}
	ChangeState("destroy");
}

void CAIDestroyable::DestroyState(float dt)
{
	change_color(VEC4(1, 0, 1, 1));
	acum_time += DT;
	if (acum_time >= recover_time)
	{
		acum_time = 0;
		change_mesh(0);
		TCompTransform *mypos = getMyTransform();
		if (!mypos) {
			return;
		}

		TCompCollider* comp_collider = get<TCompCollider>();
		if (comp_collider)
		{
			comp_collider->config.group = EnginePhysics.getFilterByName("scenario");
			comp_collider->config.mask = EnginePhysics.getFilterByName("all");
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