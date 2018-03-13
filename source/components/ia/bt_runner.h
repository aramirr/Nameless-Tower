#ifndef _BT_RUNNER_INC
#define _BT_RUNNER_INC

#include "bt.h"
#include "entity/common_msgs.h"



class bt_runner:public bt, public TCompBase
	{
	VEC3 appearing_position;
	std::queue<VEC3> jump_positions;
	float chase_distance;
	float attack_distance;
	float distance_to_player = 0.f;
	float speed_factor;
	float gravity;
	float jump_speed;
	float tower_radius;
	float jump_end;
	float jump_altitude;
	float y_speed_factor = 0.f;

	bool going_right = false;
	bool jumping = false;
	bool freeze = false;
	bool run = false;
	
	VEC3 tower_center = VEC3::Zero;
	std::string actual_state;

	DECL_SIBLING_ACCESS();

	public:
		void load(const json& j, TEntityParseContext& ctx);
		void debugInMenu();

		void on_player_jump(const TMsgJump& msg);
		void appear(const TMsgRunnerAppear& msg);
		void stop(const TMsgRunnerStop& msg);

		static void registerMsgs();


		void create(string);

		int actionDisappear();
		int actionIdle();
		int actionAppear();
		int actionAttack();
		int actionChase();
		int actionHide();

		bool conditionIdle();
		bool conditionDisappear();
		bool conditionRun();
		bool conditionAttack();



		int jumping_state();
	};

#endif