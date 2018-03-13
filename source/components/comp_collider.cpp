#include "mcv_platform.h"
#include "comp_collider.h"
#include "comp_transform.h"

DECL_OBJ_MANAGER("collider", TCompCollider);

void TCompCollider::debugInMenu() {
}

void TCompCollider::load(const json& j, TEntityParseContext& ctx) {
  std::string shape = j["shape"].get<std::string>();
  if (strcmp("box", shape.c_str()) == 0)
  {
    config.shapeType = physx::PxGeometryType::eBOX;
  }
  else if (strcmp("sphere", shape.c_str()) == 0)
  {
    config.shapeType = physx::PxGeometryType::eSPHERE;
  }
  else if (strcmp("plane", shape.c_str()) == 0)
  {
    config.shapeType = physx::PxGeometryType::ePLANE;
  }
  else if (strcmp("capsule", shape.c_str()) == 0)
  {
    config.shapeType = physx::PxGeometryType::eCAPSULE;
  }
  
  config.is_character_controller = j.value("is_character_controller", false);
  config.is_dynamic = j.value("is_dynamic", false);
  config.is_trigger = j.value("is_trigger", false);
  config.radius = j.value("radius", 0.f);
  config.height = j.value("height", 0.f);
	if (j.count("offset"))
		config.offset = loadVEC3(j["offset"]);
	else config.offset = VEC3(0, 0, 0);
	

  // todo: extend this be able to parse more than group and mask
  config.group = CEngine::get().getPhysics().getFilterByName(j.value("group", "all"));
  config.mask = CEngine::get().getPhysics().getFilterByName(j.value("mask", "all"));

  if (j.count("halfExtent"))
    config.halfExtent = loadVEC3(j["halfExtent"]);
}


void TCompCollider::registerMsgs() {
  DECL_MSG(TCompCollider, TMsgEntityCreated, onCreate);
}

void TCompCollider::onCreate(const TMsgEntityCreated& msg) {
  CEngine::get().getPhysics().createActor(*this);
}