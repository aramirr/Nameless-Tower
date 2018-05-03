#include "mcv_platform.h"
#include "comp_hierarchy.h"
#include "comp_father.h"
#include "entity/entity.h"
#include "entity/entity_parser.h"
#include "components/juan/comp_transform.h"
#include "entity/common_msgs.h"

DECL_OBJ_MANAGER("hierarchy", TCompHierarchy);

using namespace physx;

void TCompHierarchy::load(const json& j, TEntityParseContext& ctx) {
  assert(j.count("parent") );
  parent_name = j.value("parent", "");
  // Relative transform is loaded as any other json transform
  CTransform::load(j);
}

void TCompHierarchy::registerMsgs() {
  DECL_MSG(TCompHierarchy, TMsgEntitiesGroupCreated, onGroupCreated);
}

void TCompHierarchy::onGroupCreated(const TMsgEntitiesGroupCreated& msg) {
  // I prefer to wait until the group is loaded to resolve my parent
  setParentEntity(msg.ctx.findEntityByName(parent_name));
}

void TCompHierarchy::debugInMenu() {
  ImGui::LabelText("Parent Name", "%s", parent_name.c_str());
  CHandle h_parent_entity = h_parent_transform.getOwner();
  if (h_parent_entity.isValid())
    h_parent_entity.debugInMenu();
  CTransform::debugInMenu();
} 

void TCompHierarchy::setParentEntity(CHandle new_h_parent) {
  CEntity* e_parent = new_h_parent;

  if (e_parent) {
    // Cache the two handles: the comp_transform of the entity I'm tracing, and my comp_transform
    h_parent_transform = e_parent->get<TCompTransform>();
    CEntity* e_my_owner = CHandle(this).getOwner();
    h_my_transform = e_my_owner->get<TCompTransform>();

	TCompFather* father = e_parent->get<TCompFather>();
	if (father) {
		CEntity* e_son = CHandle(this).getOwner();
		father->sons.push_back(e_son);
	}
  }
  else {
    // Invalidate previous contents
    h_parent_transform = CHandle();
    h_my_transform = CHandle();
  }
}

void TCompHierarchy::update(float dt) {

  // My parent world transform
  TCompTransform* c_parent_transform = h_parent_transform;
  if (!c_parent_transform)
    return;

  // My Sibling comp transform
  TCompTransform* c_my_transform = h_my_transform;
  if (c_my_transform) {
	  // Combine the current world transform with my 
	  CTransform t = c_parent_transform->combineWith(*this);
	  c_my_transform->set(t);
	  CEntity* e = h_my_transform.getOwner();
	  TCompCollider *my_collider = e->get<TCompCollider>();
	  if (my_collider) {
		  PxRigidActor* rigidActor = ((PxRigidActor*)my_collider->actor);
		  PxTransform tr = rigidActor->getGlobalPose();
		  tr.p = PxVec3(t.getPosition().x, t.getPosition().y, t.getPosition().z);
		  tr.q = PxQuat(t.getRotation().x, t.getRotation().y, t.getRotation().z, t.getRotation().w);
		  rigidActor->setGlobalPose(tr);
	  }
  }
}
