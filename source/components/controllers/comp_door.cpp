#include "mcv_platform.h"
#include "entity/entity_parser.h"
#include "comp_door.h"
#include "components/juan/comp_transform.h"
#include "components/physics/controller_filter.h"
#include "components/physics/query_filter.h"
#include "components/juan/comp_father.h"

using namespace physx;

DECL_OBJ_MANAGER("door", TCompDoor);

void TCompDoor::debugInMenu() {
}

void TCompDoor::load(const json& j, TEntityParseContext& ctx) {
  setEntity(ctx.current_entity);
  speed = j.value("speed", 1.0f);
  left = j.value("left", false);
  is_open = false;
  opening = false;
  radiants = 90.f;
  current_radiants = 0.f;
}

void TCompDoor::setEntity(CHandle new_entity) {
	h_entity = new_entity;
	assert(h_entity.isValid());
}

void TCompDoor::update(float DT) {
    if (opening && !is_open) {
        CEntity *e = h_entity;
        if (!h_entity.isValid()) return;
        TCompTransform *my_transform = e->get<TCompTransform>();
        TCompCollider *my_collider = e->get<TCompCollider>();
        VEC3 my_pos = my_transform->getPosition();
        float y, p, r;
        my_transform->getYawPitchRoll(&y, &p, &r);
        float current_rotation = speed * DT;
        y += left ? current_rotation : -current_rotation;
        current_radiants += current_rotation;
        my_transform->setYawPitchRoll(y, p, r);
        QUAT newRot = my_transform->getRotation();

        PxRigidActor* rigidActor = ((PxRigidActor*)my_collider->actor);
        PxTransform tr = rigidActor->getGlobalPose();
        tr.p = PxVec3(my_pos.x, my_pos.y, my_pos.z);
        tr.q = PxQuat(newRot.x, newRot.y, newRot.z, newRot.w);
        rigidActor->setGlobalPose(tr);

        if (current_radiants >= deg2rad(radiants)) {
            is_open = true;
        }
    }
	

	
}


void TCompDoor::destroy()
{
	if (!CHandle(this).getOwner().isValid()) {
		return;
	}
	CEntity * e = h_entity;
	/*TCompFather *father_comp = e->get<TCompFather>();
	if (father_comp) {
		for (auto h : father_comp->sons) {
			CEntity* e = h;
			e->~CEntity();
		}
	}*/

	CHandle h_col = e->get<TCompCollider>();
	TCompCollider *my_col = e->get<TCompCollider>();
	if (my_col) {
		my_col->actor->getScene()->removeActor(*my_col->actor);
		my_col->actor = nullptr;

		CEntity* e = CHandle(this).getOwner();
		assert(e);
		my_col->controller->release();
		my_col->controller = nullptr;
	}
	CEntity *e_creator = h_parent;
	TMsgWindstrike msg;
	e_creator->sendMsg(msg);	
	CHandle(this).getOwner().destroy();
}

void TCompDoor::open(const TMsgOpenDoor& msg)
{
    opening = true;
}

void TCompDoor::registerMsgs() {
    DECL_MSG(TCompDoor, TMsgOpenDoor, open);
}