#pragma once

#include "components/comp_base.h"
#include "geometry/transform.h"
#include "components/ia/ai_controller.h"


class TCompPlayerController : public TCompBase {	
  DECL_SIBLING_ACCESS();

public:
	VEC3	omnidash_vector;
    VEC2	omnidash_arrow;
    VEC3	player_position;
    bool    can_die;
    bool    run_sprite = true;
    bool    left_key = false;
    float   gravity;
    float   omni_angle;
	float   y_speed_factor = 0.f;
	float	jumping_start_height;
	float	jumping_death_height;
    float   run_time = 0;
    float   idle_time = 0;
    float   idle_max_time;
	VEC3 checkpoint;
    bool    looking_left;
    bool    camera_idle = false;
    bool    on_cinematic = false;
	bool    is_grounded;
	bool    is_falling;
	bool    is_running = false;
	int anim1 = -1;
	int anim2 = -1;
    std::string previous_camera;
    std::string previous_state;
    std::string game_state = "level_1";
	VEC3	  center;
	float	  tower_radius;
	CHandle         h_entity;
	CHandle         h_transform;        // Cached
	CHandle         h_render;        // Cached
	CHandle         h_collider;        // Cached
    Studio::EventInstance*  _sound_land;

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
		, NajaOmniPrepDnFr
		, NajaOmniPrepUpFr
		, NajaOmniPrepUpBk
		, NajaOmniPrepDnBk
        , NajaOmniDn
        , NajaOmniAr
        , NajaOmniBk
        , NajaOmniFr
        , NajaOmniFrDn
        , NajaOmniFrUp
        , NajaOmniBkUp
        , NajaOmniBkDn
		, NajaDashStrike
		, NajaDashLand
		, NajaWalk
		, NajaWindstrikeA
		, NajaDeath
		, NajaDead
		, NajaBreak
        , NajaLookDn
        , NajaLookUp
		, NajaIntro
        , NajaScare,
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