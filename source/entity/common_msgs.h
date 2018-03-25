#ifndef INC_COMMON_ENTITY_MSGS_
#define INC_COMMON_ENTITY_MSGS_

#include "msgs.h"
#include "geometry/transform.h"

struct TMsgEntityCreated {
  DECL_MSG_ID();
};

struct TMsgAssignBulletOwner {
  CHandle h_owner;
  DECL_MSG_ID();
};

struct TMsgDamage {
  CHandle h_sender;
  CHandle h_bullet;
  DECL_MSG_ID();
};

struct TMsgJump {
	VEC3 jump_position;
	DECL_MSG_ID();
};

struct TMsgRunnerAppear {
	VEC3 appearing_position;
	bool b_appear;				//If True appears, else disappears
	std::string next_state;
	DECL_MSG_ID();
};

struct TMsgRunnerStop {
	DECL_MSG_ID();
};

struct TMsgCheckpoint {
	VEC3 appearing_position;
	DECL_MSG_ID();
};

struct TMsgAttachTo {
	CHandle h_attached;
	CHandle h_attacher;
	DECL_MSG_ID();
};

struct TMsgDetachOf {
	CHandle h_attached;
	CHandle h_attacher;
	DECL_MSG_ID();
};

struct TMsgTriggerSpike{
	CHandle h_player;
	DECL_MSG_ID();
};

struct TMsgKillPlayer {
	DECL_MSG_ID();
};

struct TMsgExitPlatform {
  DECL_MSG_ID();
};

struct TMsgChangeDirection {
	VEC3 new_direction;
	bool reset_duration = false;
	DECL_MSG_ID();
};

struct TMsgGravityToggle {
	bool is_active;
	DECL_MSG_ID();
};


// Sent to all entities from a parsed file once all the entities
// in that file has been created. Used to link entities between them
struct TEntityParseContext;
struct TMsgEntitiesGroupCreated {
  const TEntityParseContext& ctx;
  DECL_MSG_ID();
};

//struct TMsgisGrounded {
//  DECL_MSG_ID();
//};
//
//struct TMsgchangeCamerainPlatform {
//  DECL_MSG_ID();
//};

struct TMsgDefineLocalAABB {
	AABB* aabb;
	DECL_MSG_ID();
};

/*
struct TMsgTimeOut {
=======


/*
struct TMsgTimeOut {
  DECL_MSG_ID();
};

struct TMsgSpawn {
  DECL_MSG_ID();
};

struct TMsgSpawnAt {
  CTransform where;
  DECL_MSG_ID();
};
*/

#endif