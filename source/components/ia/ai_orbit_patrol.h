#ifndef _AIC_PATROL
#define _AIC_PATROL

#include "ai_controller.h"

class CAIOrbitPatrol : public IAIController
{
  std::vector<VEC3> _waypoints;
  int currentWaypoint;
  float speed;
  float delay;
  float acum_delay;
  VEC3 center;
  float radius;
  bool move_left;

  DECL_SIBLING_ACCESS();

public:
  void load(const json& j, TEntityParseContext& ctx);
  void debugInMenu();

  void InitializeWaypointState();
  void NextWaypointState();
  void MoveToWaypointState(float dt);
  void WaitState(float dt);

  void Init();
  void addWaypoint(VEC3 waypoint) { _waypoints.push_back(waypoint); };
  VEC3 getWaypoint() { return _waypoints[currentWaypoint]; }
  VEC3 processWaypoint(VEC3 center, VEC3 waypoint, float distance);

};

#endif