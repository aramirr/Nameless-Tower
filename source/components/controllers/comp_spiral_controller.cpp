#include "mcv_platform.h"
#include "entity/entity_parser.h"
#include "comp_spiral_controller.h"
#include "components/comp_transform.h"

using namespace physx;

DECL_OBJ_MANAGER("spiral_controller", TCompSpiralController);

void TCompSpiralController::debugInMenu() {
}


void TCompSpiralController::load(const json& j, TEntityParseContext& ctx) {
  setEntity(ctx.current_entity);
  radius = j.value("radius", 1.0f);
  //Cambiarlo para pillarlo de una entidad global
  radius = 31.999847;
  speed = j.value("speed", 1.0f);

	h_parent = ctx.entity_starting_the_parse;
	CEntity *e_creator = h_parent;
  if (!e_creator) { return; }
  TCompTransform *t_creator = e_creator->get<TCompTransform>();
  VEC3 current_position = t_creator->getPosition();
	//Cambiarlo para pillarlo de una entidad global
	radius = 31.999847;
  center = VEC3(0, current_position.y, 0);
  offsetY = current_position.y;
  direction = t_creator->getFront();
  direction.Normalize();
}

void TCompSpiralController::setEntity(CHandle new_entity) {
	h_entity = new_entity;
	assert(h_entity.isValid());
}

void TCompSpiralController::update(float dt) {
	CEntity *e = h_entity;
	TCompTransform *my_transform = e->get<TCompTransform>();
	VEC3 delta_pos = direction * dt * speed;
	VEC3 new_pos = my_transform->getPosition() + delta_pos;
	center.y = new_pos.y;
	offsetY = new_pos.y;
	float distance = VEC3::Distance(my_transform->getPosition(), center);

	VEC3 vec_from_center = new_pos - center;
	vec_from_center.Normalize();
	if (distance > radius)
	{
		new_pos = vec_from_center * radius;
		new_pos.y = offsetY;
	}
	my_transform->lookAt(new_pos, center);

	if (set_once == false) 
	{ 
		CEntity *e_creator = h_parent;
		TCompTransform *t_creator = e_creator->get<TCompTransform>();
		is_left = t_creator->isInLeft(center);
		set_once = true;
	}

	float y, p, r;
	my_transform->getYawPitchRoll(&y, &p, &r);
	is_left ? my_transform->setYawPitchRoll(deg2rad(-90) + y, p, r) : my_transform->setYawPitchRoll(deg2rad(90)+ y, p, r);

	TCompCollider *my_collider = e->get<TCompCollider>();
	if (my_collider)
	{
		PxRigidActor* rigidActor = ((PxRigidActor*)my_collider->actor);
		PxTransform tr = rigidActor->getGlobalPose();
		tr.p = PxVec3(new_pos.x, new_pos.y, new_pos.z);
		rigidActor->setGlobalPose(tr);
	}
}

void TCompSpiralController::registerMsgs()
{
	DECL_MSG(TCompSpiralController, TMsgChangeDirection, setDirection);
}

void TCompSpiralController::setDirection(const TMsgChangeDirection& msg)
{
	direction = msg.new_direction;
	direction.Normalize();
	set_once = false;
	if (msg.reset_duration) {

	}
}