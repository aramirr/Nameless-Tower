#ifndef _AIC_PATROL
#define _AIC_PATROL

#include "ai_controller.h"

class CAILinearPatrol : public IAIController
{
  std::vector<VEC3> _waypoints;
  int currentWaypoint;
  float speed;
  float delay;
  float acum_delay;

  DECL_SIBLING_ACCESS();

public:
  void load(const json& j, TEntityParseContext& ctx);
  void debugInMenu();

  void InitializeWaypointState();
  void NextWaypointState();
  void MoveToWaypointState();
  void WaitState(float dt);

  void Init();
  void addWaypoint(VEC3 waypoint) { _waypoints.push_back(waypoint); };
  VEC3 getWaypoint() { return _waypoints[currentWaypoint]; }

};

#endif