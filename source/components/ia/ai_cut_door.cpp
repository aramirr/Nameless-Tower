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
	ChangeState("config_state");
}

void CAICutDoor::debugInMenu() {
	IAIController::debugInMenu();
}

void CAICutDoor::load(const json& j, TEntityParseContext& ctx) {
	setEntity(ctx.current_entity);
	distance = j.value("distance", 2.0f);
	Init();
}

void CAICutDoor::ConfigState() {
	CEntity* e = h_entity;
	TCompHierarchy *c_my_hierarchy = e->get< TCompHierarchy >();
	TCompTransform *c_my_transform = c_my_hierarchy->h_my_transform;
	TCompTransform *c_my_transform2 = c_my_hierarchy->h_parent_transform;
	open_position = c_my_transform->getPosition();
	open_position.y += distance;
	c_my_transform->setPosition(open_position);
	closed_position = c_my_transform2->getPosition();
	closed_position.y += 2.f;
	TCompTransform *my_t = getMyTransform();
	VEC3 p = my_t->getPosition();
	p.y += distance + 1.f;
	my_t->setPosition(p);
};

void CAICutDoor::ClosingState(float dt) {};
void CAICutDoor::OpeningState(float dt) {};
void CAICutDoor::OpenState(float dt) {};
void CAICutDoor::ClosedState(float dt) {};