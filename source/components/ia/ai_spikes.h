#ifndef _AIC_PATROL
#define _AIC_PATROL

#include "ai_controller.h"

class CAISpikes : public IAIController
{
  DECL_SIBLING_ACCESS();

public:
  void load(const json& j, TEntityParseContext& ctx);
  void debugInMenu();

  void SpikesOffState();
  void TriggerSpikeState(float dt);
  void SpikesOnState(float dt);
  void change_color(VEC4 color);

  void Init();

  bool fixed_spikes;
  float time_to_trigger;
  float time_to_return_to_off;
  float acum_time;

  static void registerMsgs();
  void doOnCollision(const TMsgTriggerSpike& msg);
};

#endif