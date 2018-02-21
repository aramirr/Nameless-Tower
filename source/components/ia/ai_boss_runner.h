#ifndef _AIC_BOSS_RUNNER
#define _AIC_BOSS_RUNNER

#include "ai_controller.h"
#include "entity/common_msgs.h"

class CAIBossRunner : public IAIController
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
	bool going_right = false;
	VEC3 tower_center = VEC3::Zero;
	std::string actual_state;

	DECL_SIBLING_ACCESS();

public:
	void load(const json& j, TEntityParseContext& ctx);
	void debugInMenu();

	
	void AppearState(float dt);
	void ChaseState(float dt);
	void AttackState();
	void DissapearState();
	void JumpingState(float dt);

	void Init();

	void onPlayerJump(const TMsgJump& msg);
	void Appear(const TMsgAppear& msg);

	static void registerMsgs();
};

#endif