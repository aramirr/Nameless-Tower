#include "mcv_platform.h"
#include "handle/handle.h"
#include "ai_cut_door.h"
#include "entity/entity_parser.h"
#include "components/juan/comp_transform.h"
#include "render/render_utils.h"
#include "modules/system/module_physics.h"
#include "components\juan\comp_hierarchy.h"

using namespace physx;

DECL_OBJ_MANAGER("ai_cut_door", CAICutDoor);

void CAICutDoor::Init()
{
	// insert all states in the map
	AddState("closed_state", (statehandler)&CAICutDoor::ClosedState);
	AddState("closing_state", (statehandler)&CAICutDoor::ClosingState);
	AddState("opening_state", (statehandler)&CAICutDoor::OpeningState);
	AddState("open_state", (statehandler)&CAICutDoor::OpenState);
	AddState("config_state", (statehandler)&CAICutDoor::ConfigState);

	// reset the state
	ChangeState("closed_state");
}

void CAICutDoor::debugInMenu() {
	IAIController::debugInMenu();
	CTransform* t = getMyTransform();
	VEC3 pos = t->getPosition();
	ImGui::DragFloat("Opening speed: %f", &opening_speed, 0.01f, 0.f, 15.f);
	ImGui::DragFloat("Closing speed: %f", &closing_speed, 0.01f, 0.f, 15.f);
	ImGui::Text("pos: %f %f %f", pos.x, pos.y, pos.z);
}

void CAICutDoor::load(const json& j, TEntityParseContext& ctx) {
	setEntity(ctx.current_entity);
	distance = j.value("distance", 2.0f);
	opening_speed = j.value("opening_speed", 0.5f);
  closing_speed = j.value("closing_speed", 0.5f);
  opened_position = j.value("opened_position", 8.f);
  closed_position = j.value("closed_position", 0.5f);
	up = j.value("up", 2.0f);
	Init();
}

void CAICutDoor::ConfigState() {
};

void CAICutDoor::ClosingState(float dt) {
	CEntity* e = h_entity;
	TCompHierarchy *my_hierarchy = e->get<TCompHierarchy>();
	VEC3 my_pos = my_hierarchy->getPosition();
  current_lerp_time += DT;

  float perc = current_lerp_time * current_lerp_time*current_lerp_time * (current_lerp_time * (6.f*current_lerp_time - 15.f) + 10.f);
	if (up) {

    my_pos.y = lerp(opened_position, 0.f, perc);
	}
	else {

    my_pos.y = lerp(opened_position, 0.f, perc);
	}
	TCompTransform *parent_transform = my_hierarchy->h_parent_transform;
	VEC3 new_world_pos = parent_transform->getPosition() + my_pos;
	TCompCollider *my_collider = getMyCollider();

	PxRigidActor* rigidActor = ((PxRigidActor*)my_collider->actor);
	PxTransform tr = rigidActor->getGlobalPose();
	tr.p = PxVec3(new_world_pos.x, new_world_pos.y, new_world_pos.z);
	rigidActor->setGlobalPose(tr);

	my_hierarchy->setPosition(my_pos);
  if (my_pos.y <= 0.f) {
    current_lerp_time = 0.f;
    ChangeState("closed_state");
  }

};

void CAICutDoor::OpeningState(float dt) {
	CEntity* e = h_entity;
	TCompHierarchy *my_hierarchy = e->get<TCompHierarchy>();
	VEC3 my_pos = my_hierarchy->getPosition();
  current_lerp_time += DT;
  float perc = current_lerp_time * current_lerp_time*current_lerp_time * (current_lerp_time * (6.f*current_lerp_time - 15.f) + 10.f);

	if (up) {

    my_pos.y = lerp(0.f, opened_position, perc);

		/*my_pos.y += opening_speed * DT;
		if (my_pos.y > distance)
			my_pos.y = distance;*/
	}
	else { 
    
    my_pos.y = lerp(opened_position, closed_position, perc);
		/*my_pos.y -= opening_speed * DT;
		if (my_pos.y < -distance)
			my_pos.y = -distance;*/
	}
	
	TCompTransform *parent_transform = my_hierarchy->h_parent_transform;
	VEC3 new_world_pos = parent_transform->getPosition() + my_pos;
	TCompCollider *my_collider = getMyCollider();

	PxRigidActor* rigidActor = ((PxRigidActor*)my_collider->actor);
	PxTransform tr = rigidActor->getGlobalPose();
	tr.p = PxVec3(new_world_pos.x, new_world_pos.y, new_world_pos.z);
	rigidActor->setGlobalPose(tr);

	my_hierarchy->setPosition(my_pos);
  if (my_pos.y >= opened_position) {
    current_lerp_time = 0.f;
    ChangeState("open_state");
  }

};
void CAICutDoor::OpenState(float dt) {
  current_lerp_time += DT;

  if (current_lerp_time >= 0.5f) {
    current_lerp_time = 0.f;
    ChangeState("closing_state");
  }

};

void CAICutDoor::ClosedState(float dt) {
  current_lerp_time += DT;

  if (current_lerp_time >= 0.5f) {
    current_lerp_time = 0.f;
    ChangeState("opening_state");
  }
};