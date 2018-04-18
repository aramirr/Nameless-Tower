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
	TMsgSetFSMVariable windMsg;
	windMsg.variant.setName("in_wind");
	windMsg.variant.setBool(true);
	attached = msg.h_attacher;
	CEntity *e_attached = attached;
	e_attached->sendMsg(windMsg);
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
	jumpMsg.variant.setName("in_wind");
	jumpMsg.variant.setBool(false);
	attached = msg.h_attacher;
	CEntity *e_attached = attached;
	e_attached->sendMsg(jumpMsg);
}
