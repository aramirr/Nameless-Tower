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
  float wake_time;

  CHandle	 attached;


  DECL_SIBLING_ACCESS();

public:
  void load(const json& j, TEntityParseContext& ctx);
  void debugInMenu();

  void InitializeWaypointState();
  void NextWaypointState();
  void MoveToWaypointState(float dt);
  void WaitState(float dt);
  void SleepState(float dt);

  void Init();
  void addWaypoint(VEC3 waypoint) { _waypoints.push_back(waypoint); };
  VEC3 getWaypoint() { return _waypoints[currentWaypoint]; }

  static void registerMsgs();
  void attachPlayer(const TMsgAttachTo& msg);
  void detachPlayer(const TMsgDetachOf& msg);

};

#endif