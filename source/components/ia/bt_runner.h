#ifndef _BT_RUNNER_INC
#define _BT_RUNNER_INC

#include "bt.h"
#include "entity/common_msgs.h"



class bt_runner:public bt
	{
	
	float attack_distance;
    float debug_timer = 0.f;

    bool b_disappear = false;
    bool b_appear = false;
    bool b_chase = false;
    bool b_recular = false;
    bool on_wall = false;

    VEC3 appearing_position;
	
	VEC3 tower_center = VEC3::Zero;
	std::string actual_state;

	DECL_SIBLING_ACCESS();

	public:
		void load(const json& j, TEntityParseContext& ctx);
		void debugInMenu();

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

	};

#endif