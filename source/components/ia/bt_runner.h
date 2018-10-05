#ifndef _BT_RUNNER_INC
#define _BT_RUNNER_INC

#include "bt.h"
#include "entity/common_msgs.h"



class bt_runner:public bt
	{

  // Animations Variables
  enum ERunnerAnimations {
		RunnerAparece = 0
    , RunnerAttack
		, RunnerGiro
		, RunnerScream
		, RunnerScreamShort
		, RunnerIdle
    , RunnerJumpLand
    , RunnerJumpLoop
    , RunnerJumpShort
    , RunnerJumpUp
    , RunnerLookOut
		, RunnerRecular
		, RunnerRun
    , RunnerRunCerca,
    ERunnerAnimations
  };

  std::string anim_state = "";
  int anim_id;

  // Pathfinding variables
	typedef pair<float, int> Warc;
	struct waypoint {
		int id;
		VEC3 position;
		std::string type;
		std::vector<Warc> neighbours;
	};

  std::vector<int> path;
  std::vector<waypoint> waypoints_map;
  int actual_waypoint = -1;
  int next_waypoint;
	int second_closest_waypoint;
	float recalculate_timer = 0.f;
	
  // Condition variables
  bool b_disappear = false;
  bool b_appear = false;
  bool b_chase = false;
  bool b_recular = false;
  bool on_wall = false;
	bool b_chase_player = false;

  // Logic variables
	bool going_right = false;
	bool going_up = true;
  bool on_jump = false;
  bool chase_started = false;
  bool anim_debug_changed = false;
  float attack_distance;
  float debug_timer = 0.f;
  float speed = 2.0f;
	float Vx;
	float Vy;
	float gravity = 15.f;
	
  // Other variables
  VEC3 appearing_position;
	VEC3 tower_center = VEC3::Zero;
	std::string actual_state;
	VEC3 top_jump_position = VEC3::Zero;
	std::string target = "waypoint";
  VEC3 last_pos = VEC3::Zero;

  // Animation functions
  void change_animation(int animation_id, bool is_action, float in_delay, float out_delay, bool clear);
  void clear_animations(float out_delay);
  void remove_animation(int animation_id);

  // Pathfinding functions
	void recalculate_path();
	void calculate_top_jump_position(VEC3 target_position);
	float distance_x_z(VEC3 v1, VEC3 v2);
	float distance_arc(VEC3 v1, VEC3 v2); // DIstance in radians between two points on a circumference
	void calculate_distances_graph();
  void findPath(int origin, int destiny);
	int findClosestWaypoint(VEC3 position);
	int findSecondClosestWaypoint(VEC3 position, int closest_waypoint_id);

  // BT
  void create(string);

  // Actions
  int actionStop();
  int actionScream();
  int actionDisappear();
  int actionRecular();
  int actionRecover();
  int actionAttack();
  int actionChase();
  int actionAppear();
  int actionAppearPose();
  int actionHide();

  // Conditions
  bool conditionDisappear();
  bool conditionChase();
  bool conditionRecular();
  bool conditionAttack();
  bool conditionAppear();
  
  // Other
  void killPlayer();
  void chase_waypoint();
  void chase_player();
  void walk();
  void jump();
  void addGravity();

	DECL_SIBLING_ACCESS();

	public:

  void debugInMenu();
	void load(const json& j, TEntityParseContext& ctx);
  void load_waypoint(const json& j);

 
  // Messages
	static void registerMsgs();
  void appear(const TMsgRunnerAppear& msg);
  void disappear(const TMsgRunnerDisappear& msg);

	};

#endif