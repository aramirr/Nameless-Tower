#pragma once

#include "comp_base.h"
#include "geometry/transform.h"
#include "ia/ai_controller.h"

class TCompPlayerController : public IAIController {
	float   speed_factor = 8.0f;
  VEC3    speed;
	VEC3	  center;
	VEC3	  omnidash_vector;
  float	  tower_radius;
	float   dashing_max;
	float   dashing_amount;
	float   max_jump;
	float   jump_end;
	float   gravity;
	float   jump_speed;
	float   omnidash_timer = 0;
	float   omnidash_max_time;
	float		omnidashing_ammount;
	float		omnidashing_max_ammount;
	int     dashing_speed;
	bool    looking_left;
	bool    is_grounded;
	bool		can_omni;
	CHandle	 attached;

  DECL_SIBLING_ACCESS();

public:
  void debugInMenu();
  void load(const json& j, TEntityParseContext& ctx);

  // IA
	void initial_state(float dt);
	void idle_state(float dt);
  void running_state(float dt);
  void jumping_state(float dt);
	void omnidashing_state(float dt);
	void omnidashing_jump_state(float dt);
	void dashing_state(float dt);
	void dead_state(float dt);
	void move_player(bool left, bool change_orientation, float dt, float gravity);
	static void registerMsgs();
	void attachPlayer(const TMsgAttachTo& msg);
	void detachPlayer(const TMsgDetachOf& msg);

  void init();
};