#ifndef _AIC_PATROL
#define _AIC_PATROL

#include "ai_controller.h"

class CAIDestroyable : public IAIController
{
  DECL_SIBLING_ACCESS();

public:
  void load(const json& j, TEntityParseContext& ctx);
  void debugInMenu();

  void IdleState();
  void TriggerDestroyState(float dt);
  void DestoyState();

  void Init();

  std::string trigger_actor;
  float destroy_time;
  float acum_time;

};

#endif