#pragma once

#include "components/comp_base.h"
#include "geometry/transform.h"
#include "components/ia/ai_controller.h"


class TCompPlayerController : public TCompBase {	
  DECL_SIBLING_ACCESS();

public:

	VEC3	omnidash_vector;
	VEC2	omnidash_arrow;
	float   gravity;
	float   y_speed_factor = 0.f;
	float	jumping_start_height;
	float	jumping_death_height;
	VEC3 checkpoint;
	bool    looking_left;
	bool    is_grounded;
	bool    is_falling;
	bool    is_running = false;
	int anim1 = -1;
	int anim2 = -1;

	VEC3	  center;
	float	  tower_radius;
	CHandle         h_entity;
	CHandle         h_transform;        // Cached
	CHandle         h_render;        // Cached
	CHandle         h_collider;        // Cached

	enum EAnimations {
		NajaRun = 0
		, NajaIdle
		, NajaGlideStrike
		, NajaGlideLoop
		, NajaJumpUp
		, NajaJumpLoop
		, NajaJumpLand
		, NajaOmniPrepDn
		, NajaOmniPrepUp
		, NajaOmniPrepBk
		, NajaOmniPrepFr
		, NajaOmniPrepFrDn
		, NajaOmniPrepFrUp
		, NajaOmniPrepFrBk
		, NajaOmniPrepBkDn
		, NajaOmniAb
		, NajaOmniAr
		, NajaOmniDe
		, NajaOmniFr
		, NajaDashStrike
		, NajaDashLand
		, NajaWalk
		, NajaWindstrikeA
		, NajaDeath
		, NajaDead
		, NajaBreak,
		EAnimations
	};

  void debugInMenu();
  void load(const json& j, TEntityParseContext& ctx);
	void change_animation(int animation_id, bool is_action, float in_delay, float out_delay, bool clear);
	void clear_animations(float out_delay);
	void remove_animation(int animation_id);


	void move_player(bool left, bool change_orientation, float dt, float y_speed, float x_speed);
	bool isForward() { return looking_left; };
	bool isGrounded() { return is_grounded; }

	void setCheckpoint(const TMsgCheckpoint& msg);
	
  void init();

	TCompTransform * getMyTransform();
	TCompRender* getMyRender();
	TCompCollider* getMyCollider();

private:

  bool rayWall;

	void setEntity(CHandle new_entity);
};