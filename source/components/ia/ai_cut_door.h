#ifndef _AIC_CUTDOOR
#define _AIC_CUTDOOR

#include "ai_controller.h"

class CAICutDoor : public IAIController
{
	bool up;
	float distance;
  float opening_speed;
  float closing_speed;

  float opened_position;
  float closed_position;
  float current_lerp_time = 0.f;

  DECL_SIBLING_ACCESS();

public:
  void load(const json& j, TEntityParseContext& ctx);
  void debugInMenu();

  void ClosedState(float dt);
  void ClosingState(float dt);
  void OpeningState(float dt);
  void OpenState(float dt);
  void ConfigState();

  void Init();

};

#endif