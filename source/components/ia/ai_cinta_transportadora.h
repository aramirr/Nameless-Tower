#ifndef _AIC_PATROL
#define _AIC_PATROL

#include "ai_controller.h"

struct TConfig {
	bool state_left;
	float state_speed;
	float state_time;
};

class CAICintaTransportadora : public IAIController
{
 
  CHandle attached;
  float it_config = 0;
  std::vector<TConfig> config_states;

  DECL_SIBLING_ACCESS();

public:
  void load(const json& j, TEntityParseContext& ctx);
  void debugInMenu();

  void NextConfigState();
  void MovingState(float dt);
  void load_config();

  void Init();
  
  static void registerMsgs();
  void attachPlayer(const TMsgAttachTo& msg);
  void detachPlayer(const TMsgDetachOf& msg);
};

#endif