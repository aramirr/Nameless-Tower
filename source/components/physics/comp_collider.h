#pragma once

#include "components/comp_base.h"
#include "PxPhysicsAPI.h"
#include "entity/common_msgs.h"

using namespace physx;

struct TMsgTriggerEnter {
	CHandle h_other_entity;
	DECL_MSG_ID();
};

struct TMsgColliderEnter {
	std::string h_other_entity;
	DECL_MSG_ID();
};

struct TMsgTriggerExit {
  CHandle h_other_entity;
  DECL_MSG_ID();
};

class CCollisionMesh;

class TCompCollider: public TCompBase {
public:

  struct TConfig
  {
    VEC3 halfExtent;
    
    //todo implement
    VEC3 offset;

    float radius;
    float height;
    physx::PxGeometryType::Enum shapeType;
    unsigned int group;
    unsigned int mask;
    bool is_dynamic;
    bool is_trigger;
    bool is_character_controller;
		const CCollisionMesh* col_mesh = nullptr;
  };

	std::string debug_name;
	int debug_number;
  TConfig config;
  PxActor* actor = nullptr;
	PxController* controller = nullptr;
  void debugInMenu();
  static void registerMsgs();
  void load(const json& j, TEntityParseContext& ctx);
  void onCreate(const TMsgEntityCreated& msg);
  void OnColliderEnter(const TMsgColliderEnter& msg);
	~TCompCollider();

  void setupFiltering(PxU32 filterGroup, PxU32 filterMask);

	void update(float dt);
};