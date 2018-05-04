#include "mcv_platform.h"
#include "comp_collider.h"
#include "components/juan/comp_transform.h"
#include "components/juan/comp_hierarchy.h"
#include "render/mesh/collision_mesh.h"

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
	else if (shape == "convex_mesh")
	{
		config.shapeType = physx::PxGeometryType::eCONVEXMESH;
		std::string mesh_src = j.value("mesh_src", "");
		config.col_mesh = Resources.get(mesh_src)->as<CCollisionMesh>();
	}
	else if (shape == "tri_mesh")
	{
		config.shapeType = physx::PxGeometryType::eTRIANGLEMESH;
		std::string mesh_src = j.value("mesh_src", "");
		config.col_mesh = Resources.get(mesh_src)->as<CCollisionMesh>();
	}
  
  config.is_character_controller = j.value("is_character_controller", false);
  config.is_dynamic = j.value("is_dynamic", false);
  config.is_trigger = j.value("is_trigger", false);
  config.radius = j.value("radius", 0.f);
  config.height = j.value("height", 0.f);
	if (j.count("offset"))
		config.offset = loadVEC3(j["offset"]);
	else config.offset = VEC3(0, 0, 0);
	
	debug_name = j.value("debug_name", "default");
	


  // todo: extend this be able to parse more than group and mask
  config.group = CEngine::get().getPhysics().getFilterByName(j.value("group", "all"));
  config.mask = CEngine::get().getPhysics().getFilterByName(j.value("mask", "all"));

  if (j.count("halfExtent"))
    config.halfExtent = loadVEC3(j["halfExtent"]);

}


void TCompCollider::registerMsgs() {
  DECL_MSG(TCompCollider, TMsgEntityCreated, onCreate);
  DECL_MSG(TCompCollider, TMsgColliderEnter, OnColliderEnter);
}

void TCompCollider::OnColliderEnter(const TMsgColliderEnter& msg) {
	int aux = 1;
	/*CHandle h_other_entity = msg.h_other_entity;
	CEntity* e_other_entity = h_other_entity;
	std::string other_entity_name = e_other_entity->getName();*/
}

void TCompCollider::onCreate(const TMsgEntityCreated& msg) {
  CEngine::get().getPhysics().createActor(*this);
}

TCompCollider::~TCompCollider() {
	if (actor && actor->getScene())
		actor->getScene()->removeActor(*actor);
	actor = nullptr;
	CHandle h_this = CHandle(this);
	CEntity* e = CHandle(this).getOwner();
	dbg("TCompCollider of entity %s dtor of ctroller %p. H_this = %08x D_name %s.\n", e ?  e->getName() : "<null>", controller, h_this.asUnsigned(), debug_name.c_str());
	if (controller != NULL && controller) {
		controller->release();
		controller = nullptr;
	}
}

void TCompCollider::setupFiltering(PxU32 filterGroup, PxU32 filterMask) {
	// llamada => my_collider->setupFIltering(my_collider->config.group, my_collider->config.mask);
	CModulePhysics module_physics = CEngine::get().getPhysics();
	module_physics.setupFiltering(static_cast<PxRigidActor*>(actor), filterGroup, filterMask);
}

void TCompCollider::update(float dt) {
	CEntity* e = CHandle(this).getOwner();
	if (e)
		debug_name = e->getName();
}