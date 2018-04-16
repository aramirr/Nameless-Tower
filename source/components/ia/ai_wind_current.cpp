#include "mcv_platform.h"
#include "handle/handle.h"
#include "ai_wind_current.h"
#include "entity/entity_parser.h"
#include "components/juan/comp_transform.h"
#include "components/fsm/comp_fsm.h"
#include "render/render_utils.h"
#include "modules/system/module_physics.h"
#include "components/player/comp_player_controller.h"

using namespace physx;

DECL_OBJ_MANAGER("ai_fan", CAIFan);


void CAIFan::debugInMenu() {

	IAIController::debugInMenu();
	ImGui::DragFloat("Force %f", &force);
}

void CAIFan::Init()
{
	// insert all states in the map
	AddState("idle", (statehandler)&CAIFan::IdleState);

	// reset the state
	ChangeState("idle");
}

void CAIFan::load(const json& j, TEntityParseContext& ctx) {
	setEntity(ctx.current_entity);

	force = j.value("force", 0.025);
	Init();
}

void CAIFan::IdleState(float)
{
	if (attached.isValid()) {
		TCompTransform *my_transform = get<TCompTransform>();
		if (!my_transform)
		{
			return;
		}
		CEntity *e_attached = attached;
		TCompTransform *e_transform = e_attached->get<TCompTransform>();
		TCompCollider *e_col = e_attached->get<TCompCollider>();
		if (!e_col) {
			return;
		}
		//Mitigate force by distance from Center of the fan
		//float distance_from_transform = VEC3::Distance(e_transform->getPosition(), my_transform->getPosition());
		VEC3 direction = my_transform->getUp() * force;
		e_col->controller->move(physx::PxVec3(direction.x, direction.y, direction.z), 0.f, DT, physx::PxControllerFilters());

		PxRigidActor* rigidActor = ((PxRigidActor*)e_col->actor);
		PxTransform tr = rigidActor->getGlobalPose();
		e_transform->setPosition(VEC3(tr.p.x, tr.p.y, tr.p.z));
	}
}


void CAIFan::registerMsgs() {
	DECL_MSG(CAIFan, TMsgAttachTo, attachPlayer);
	DECL_MSG(CAIFan, TMsgDetachOf, detachPlayer);
}

void CAIFan::attachPlayer(const TMsgAttachTo& msg) {
	/*attached = msg.h_attacher;
	CEntity *e_attached = attached;
	TMsgGravityToggle disable_player_gravity_message;
	disable_player_gravity_message.is_active = false;
	e_attached->sendMsg(disable_player_gravity_message);*/
	TMsgSetFSMVariable jumpMsg;
	jumpMsg.variant.setName("in_current");
	jumpMsg.variant.setBool(true);	
	attached = msg.h_attacher;
	CEntity *e_attached = attached;
	e_attached->sendMsg(jumpMsg);
}

void CAIFan::detachPlayer(const TMsgDetachOf& msg) {
	//CHandle inc_attached = msg.h_attacher;
	//CEntity *e_inc_attached = inc_attached;
	//if (inc_attached == attached)
	//{
	/*
	CEntity *e_attached = attached;
	TMsgGravityToggle enable_player_gravity_message;
	enable_player_gravity_message.is_active = true;
	e_attached->sendMsg(enable_player_gravity_message);
	attached = CHandle();
	//}
	*/
	TMsgSetFSMVariable jumpMsg;
	jumpMsg.variant.setName("in_current");
	jumpMsg.variant.setBool(false);
	attached = msg.h_attacher;
	CEntity *e_attached = attached;
	e_attached->sendMsg(jumpMsg);
}
