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
	bool going_right = false;
	float debug = 70.0f;
	VEC3 tower_center = VEC3::Zero;

	DECL_SIBLING_ACCESS();

public:
	void load(const json& j, TEntityParseContext& ctx);
	void debugInMenu();

	void ResetWptsState();
	void HideState();
	void AppearState();
	void ChaseState();
	void AttackState();
	void DissapearState();
	void NextWptState();
	void DieState();

	void Init();

	void addWaypoint(VEC3 waypoint) { _waypoints.push_back(waypoint); };
	VEC3 getWaypoint() { return _waypoints[currentWaypoint]; }
};

#endif