#pragma once

#include "components/comp_base.h"
#include "geometry/transform.h"
#include "components/ia/ai_controller.h"


class TCompPlayerController : public TCompBase {	
  DECL_SIBLING_ACCESS();

  //float DT;

public:
	float   current_x_speed_factor = 2.0f;
	float   x_speed_factor = 2.0f;
	float   y_speed_factor = 0.f;
	VEC3    speed;
	VEC3	omnidash_vector;
	VEC2	omnidash_arrow;
	float   dashing_max;
	float   dashing_amount;
	float   jump_end;
	float   gravity;
	float   jump_speed;
	float   omnidash_timer = 0;
	float   omnidash_max_time;
	float	omnidashing_ammount;
	float	omnidashing_max_ammount;
	float	jumping_start_height;
	float	jumping_death_height;
	int     dashing_speed;
	bool	can_omni;
	bool	can_dash;
	float checkpoint_yaw;

	VEC3 checkpoint;
	bool    looking_left;
	bool    is_grounded;
	VEC3	  center;
	float	  tower_radius;
	CHandle         h_entity;
	CHandle         h_transform;        // Cached
	CHandle         h_render;        // Cached
	CHandle         h_collider;        // Cached
	
	float DT;

  void debugInMenu();
  void load(const json& j, TEntityParseContext& ctx);
	void change_mesh(int mesh_index);

  // IA
	/*void idle_state(float dt);
  void running_state(float dt);
  void jumping_state(float dt);
	void omnidashing_state(float dt);
	void omnidashing_jump_state(float dt);
	void dashing_state(float dt);
	void dead_state(float dt);
	*/
	void move_player(bool left, bool change_orientation, float dt, float y_speed);
	bool isForward() { return looking_left; };
	bool isGrounded() { return is_grounded; }

	/*
	static void registerMsgs();
	void killPlayer(const TM sgKillPlayer& msg);
	*/
	void setCheckpoint(const TMsgCheckpoint& msg);
	
  void init();

	TCompTransform * getMyTransform();
	TCompRender* getMyRender();
	TCompCollider* getMyCollider();

private:
	void setEntity(CHandle new_entity);
};