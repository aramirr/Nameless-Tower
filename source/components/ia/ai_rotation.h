#ifndef _AIC_ROTATOR
#define _AIC_ROTATOR

#include "ai_controller.h"


#define X 0
#define Y 1
#define Z 2

struct TConfig {
	float axis;
	bool increase;
	float speed;
	float wait_time;
	float radiants;
};

class CAIRotator : public IAIController
{
 
  CHandle	 attached;
  float current_time = 0.f;
  float current_radiants = 0.f;
  float it_config = 0;
  std::vector<TConfig> config_states;

  DECL_SIBLING_ACCESS();

public:
  void load(const json& j, TEntityParseContext& ctx);
  void debugInMenu();

  void NextConfigState();
  void RotateState(float dt);
  void StopState(float dt);

  void Init();
  
  static void registerMsgs();
  void attachPlayer(const TMsgAttachTo& msg);
  void detachPlayer(const TMsgDetachOf& msg);
  void activateRotation(const TMsgRotate& msg);
};

#endif