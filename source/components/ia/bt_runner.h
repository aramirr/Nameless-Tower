#ifndef _BT_RUNNER_INC
#define _BT_RUNNER_INC

#include "bt.h"
#include "entity/common_msgs.h"



class bt_runner:public bt
	{

	struct waypoint {
		int id;
		VEC3 position;
		std::string type;
		std::vector<int> neighbours;
	};
	
	float attack_distance;
  float debug_timer = 0.f;

  bool b_disappear = false;
  bool b_appear = false;
  bool b_chase = false;
  bool b_recular = false;
  bool on_wall = false;

  bool going_right = false;

	std::vector<waypoint> path;
	std::vector<waypoint> waypoints_map;
	waypoint actual_waypoint;
	waypoint next_waypoint;

  VEC3 appearing_position;
	VEC3 tower_center = VEC3::Zero;
	std::string actual_state;

  void get_next_waypoint();

	DECL_SIBLING_ACCESS();

	public:
		void load(const json& j, TEntityParseContext& ctx);
		void debugInMenu();
    void load_waypoint(const json& j);

    void appear(const TMsgRunnerAppear& msg);
    void disappear(const TMsgRunnerDisappear& msg);

		static void registerMsgs();


		void create(string);

    int actionStop();
    int actionScream();
    int actionDisappear();
    int actionRecular();
    int actionRecover();
    int actionAttackWall1();
    int actionAttackWall2();
    int actionAttackFloor1();
    int actionAttackFloor2();
    int actionChase();
    int actionAppear();
    int actionHide();

    bool conditionDisappear();
    bool conditionChase();
    bool conditionRecular();
    bool conditionAttack();
    bool conditionAttackWall();
    bool conditionAttackFloor();
    bool conditionAppear();

    void killPlayer();

		void chase();
		void findPath(int origin, int destiny, std::vector<int>& path);
		int findClosestWaypoint(VEC3 position);
		void go_to_next_waypoint();
		void walk();
		void jump();

	};

#endif