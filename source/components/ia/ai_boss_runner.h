#ifndef _AIC_BOSS_RUNNER
#define _AIC_BOSS_RUNNER

#include "ai_controller.h"
#include "entity/common_msgs.h"

class CAIBossRunner : public IAIController
{
	std::vector<VEC3> _waypoints;
	int currentWaypoint;
	float chase_distance;
	float attack_distance;
	float distance_to_player = 0.f;
	float speed_factor;
	float tower_radius;
	bool going_right = false;
	VEC3 tower_center = VEC3::Zero;

	float aux_count = 0.f;

	DECL_SIBLING_ACCESS();

public:
	void load(const json& j, TEntityParseContext& ctx);
	void debugInMenu();

	void ResetWptsState();
	void HideState();
	void AppearState();
	void ChaseState(float dt);
	void AttackState();
	void DissapearState();
	void NextWptState(float dt);
	void DieState();

	void Init();

	void onPlayerJump(const TMsgJump& msg);

	static void registerMsgs();

	void addWaypoint(VEC3 waypoint) { _waypoints.push_back(waypoint); };
	VEC3 getWaypoint() { return _waypoints[currentWaypoint]; }
};

#endif