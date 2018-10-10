#include "mcv_platform.h"
#include "handle/handle.h"
#include "ai_compresora.h"
#include "entity/entity_parser.h"
#include "components/juan/comp_transform.h"
#include "render/render_utils.h"
#include "modules/system/module_physics.h"
#include "components/physics/controller_filter.h"
#include "components/physics/query_filter.h"
#include "../sound/comp_sound.h"

using namespace physx;

DECL_OBJ_MANAGER("ia_compressora_kill", CAICompresora);

void CAICompresora::Init()
{
	// insert all states in the map
	AddState("sleep", (statehandler)&CAICompresora::SleepState);
	AddState("move_up", (statehandler)&CAICompresora::MoveUpState);
	AddState("move_down", (statehandler)&CAICompresora::MoveDownState);
	AddState("wait_state", (statehandler)&CAICompresora::WaitState);

	// reset the state
	ChangeState("sleep");
}

void CAICompresora::debugInMenu() {
	IAIController::debugInMenu();
	ImGui::DragFloat("Speed %f", &speed);
	ImGui::DragFloat("Delay %f", &delay);
	ImGui::Text("Acum Delay %f", acum_delay);
}

void CAICompresora::load(const json& j, TEntityParseContext& ctx) {
	setEntity(ctx.current_entity);

	Init();

  rol = j.value("rol", "son");
  if (rol == "son")
    other_name = j.value("other_name", "");
  else if (rol == "father") {
    trigger1 = j.value("trigger1", "");
    trigger2 = j.value("trigger2", "");
    maxY = j.value("maxY", 1.0f);
    minY = j.value("minY", 0.0f);
    wake_time = j.value("wake", 0.0f);
    speed = j.value("speed", 2.0f);
    delay = j.value("delay", 2.0f);
  }
}

void CAICompresora::MoveUpState()
{
  TCompTransform *my_transform = get< TCompTransform >();

  VEC3 my_pos = my_transform->getPosition();
  VEC3 new_pos = my_pos;
  new_pos.y += speed * DT;
  my_transform->setPosition(new_pos);

  TCompCollider* comp_collider = get<TCompCollider>();
  if (comp_collider)
  {

    PxRigidActor* rigidActor = ((PxRigidActor*)comp_collider->actor);
    PxTransform tr = rigidActor->getGlobalPose();
    if (new_pos.y >= maxY) {
      tr.p = PxVec3(new_pos.x, maxY, new_pos.z);
    }
    else {
      tr.p = PxVec3(new_pos.x, new_pos.y, new_pos.z);
    }
    rigidActor->setGlobalPose(tr);
  }
  if (new_pos.y >= maxY) {
    going_up = false;
    acum_delay = 0;
    ChangeState("wait_state");
  }
}

void CAICompresora::MoveDownState()
{
  TCompTransform *my_transform = get< TCompTransform >();

  VEC3 my_pos = my_transform->getPosition();
  VEC3 new_pos = my_pos;
  new_pos.y -= speed * DT;
  my_transform->setPosition(new_pos);

  TCompCollider* comp_collider = get<TCompCollider>();
  if (comp_collider)
  {

    PxRigidActor* rigidActor = ((PxRigidActor*)comp_collider->actor);
    PxTransform tr = rigidActor->getGlobalPose();
    if (new_pos.y <= minY) {
      tr.p = PxVec3(new_pos.x, minY, new_pos.z);
    }
    else {
      tr.p = PxVec3(new_pos.x, new_pos.y, new_pos.z);
    }

    rigidActor->setGlobalPose(tr);
  }
  if (new_pos.y <= minY) {
    going_up = true;
    acum_delay = 0;
    ChangeState("wait_state");
  }
}

void CAICompresora::SleepState()
{
  if (rol == "father") {
    acum_delay += DT;
    if (wake_time < acum_delay) {
      ChangeState("move_down");
      CEntity* e = CHandle(this).getOwner();
      TMsgPlaySound msg;
      e->sendMsg(msg);
    }
  }
}

void CAICompresora::WaitState()
{
  acum_delay += DT;
  if (delay < acum_delay) {
	if (trigger1 != "" && trigger2 != "") {
		TMsgChangeDirectionUp msg;
		msg.b_up = going_up;
		CEntity* e1 = getEntityByName(trigger1);
		CEntity* e2 = getEntityByName(trigger2);
		e1->sendMsg(msg);
		e2->sendMsg(msg);
	}
    if (going_up) {
      ChangeState("move_up");
      CEntity* e = CHandle(this).getOwner();
      TMsgPlaySound msg;
      e->sendMsg(msg);
    }
    else if (!going_up) {
      ChangeState("move_down");
      CEntity* e = CHandle(this).getOwner();
      TMsgPlaySound msg;
      e->sendMsg(msg);
    }
  }
}


void CAICompresora::checkIfTouchingPlayer(const TMsgCheckPlayerIn& msg) {
  if (player_in && !going_up) {
    CEntity *player = (CEntity *)getEntityByName("The Player");
    TMsgSetFSMVariable deadMsg;
    deadMsg.variant.setName("hit");
    deadMsg.variant.setBool(true);
    player->sendMsg(deadMsg);
    TCompSound* sound = get<TCompSound>();
    sound->playSound("hit");
  }
}

void CAICompresora::playerIn(const TMsgPlayerIn& msg) {
  player_in = true;
  TMsgCheckPlayerIn msg2;
  CEntity* e = getEntityByName(other_name);
  e->sendMsg(msg2);
}

void CAICompresora::playerOut(const TMsgPlayerOut& msg) {
  player_in = false;
}

void CAICompresora::changeDirection(const TMsgChangeDirectionUp& msg) {
  going_up = msg.b_up;
}

void CAICompresora::registerMsgs() {
  DECL_MSG(CAICompresora, TMsgCheckPlayerIn, checkIfTouchingPlayer);
  DECL_MSG(CAICompresora, TMsgChangeDirectionUp, changeDirection);
  DECL_MSG(CAICompresora, TMsgPlayerIn, playerIn);
  DECL_MSG(CAICompresora, TMsgPlayerOut, playerOut);
}
