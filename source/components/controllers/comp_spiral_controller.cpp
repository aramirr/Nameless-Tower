#include "mcv_platform.h"
#include "entity/entity_parser.h"
#include "comp_spiral_controller.h"
#include "components/juan/comp_transform.h"
#include "components/physics/controller_filter.h"
#include "components/physics/query_filter.h"
#include "components/juan/comp_father.h"

using namespace physx;

DECL_OBJ_MANAGER("spiral_controller", TCompSpiralController);

void TCompSpiralController::debugInMenu() {
	ImGui::Text("Life: %f",life);
}

void TCompSpiralController::load(const json& j, TEntityParseContext& ctx) {
  setEntity(ctx.current_entity);
  radius = j.value("radius", 1.0f);
  //Cambiarlo para pillarlo de una entidad global
  radius = 31.999847;
  speed = j.value("speed", 1.0f);
	life = j.value("life", 2);

	h_parent = ctx.entity_starting_the_parse;
	CEntity *e_creator = h_parent;
  if (!e_creator) { return; }
  TCompTransform *t_creator = e_creator->get<TCompTransform>();
  VEC3 current_position = t_creator->getPosition();
	//Cambiarlo para pillarlo de una entidad global
	radius = 31.999847;
  center = VEC3(0, current_position.y, 0);
  offsetY = current_position.y;
  if (ctx.front.x) {
	  direction = ctx.front;
	  set_once = false;
  }
  else {
	  direction = t_creator->getFront();
  }

  direction.Normalize();
  if (j.count("sound")) {
      Studio::EventDescription* event_description = NULL;
      std::string event_name = j["sound"];
      EngineSound.system->getEvent(event_name.c_str(), &event_description);
      event_description->createInstance(&_sound);
      _sound->start();
  }
}

void TCompSpiralController::setEntity(CHandle new_entity) {
	h_entity = new_entity;
	assert(h_entity.isValid());
}

void TCompSpiralController::update(float DT) {
	CEntity *e = h_entity;
	if (!h_entity.isValid()) return;
	TCompTransform *my_transform = e->get<TCompTransform>();
	VEC3 start_pos = my_transform->getPosition();
	VEC3 delta_pos = direction * DT * speed;
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
	VEC3 move_vector = new_pos - start_pos;

	if (set_once == false) 
	{ 
		CEntity *e_creator = h_parent;
		TCompTransform *t_creator = e_creator->get<TCompTransform>();
		is_left = t_creator->isInLeft(center);
		set_once = true;
	}

	TCompCollider *my_collider = e->get<TCompCollider>();
	if (my_collider)
	{
		PxShape* player_shape;
		my_collider->controller->getActor()->getShapes(&player_shape, 1);
		PxFilterData filter_data = player_shape->getSimulationFilterData();
		ControllerFilterCallback *filter_controller = new ControllerFilterCallback();
		BasicQueryFilterCallback *query_filter = new BasicQueryFilterCallback();
		PxControllerCollisionFlags flags = my_collider->controller->move(PxVec3(move_vector.x, move_vector.y, move_vector.z), 0.f, DT, PxControllerFilters(&filter_data, query_filter, filter_controller));
		
		if (flags.isSet(PxControllerCollisionFlag::eCOLLISION_DOWN) or flags.isSet(PxControllerCollisionFlag::eCOLLISION_UP) or flags.isSet(PxControllerCollisionFlag::eCOLLISION_SIDES))
		{
			this->destroy();
			return;
		}
	}

	life -= DT;
	if (life <= 0) 
		destroy();
}

void TCompSpiralController::registerMsgs()
{
  DECL_MSG(TCompSpiralController, TMsgChangeDirection, setDirection);
  DECL_MSG(TCompSpiralController, TMsgDestroy, destroyWindstrike);
}

void TCompSpiralController::destroyWindstrike(const TMsgDestroy& msg) {
  life = -1.f;
}

void TCompSpiralController::setDirection(const TMsgChangeDirection& msg)
{
	direction = msg.new_direction;
	direction.Normalize();
	set_once = false;
	if (msg.reset_duration) {

	}
}

void TCompSpiralController::destroy()
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
    _sound->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT);

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