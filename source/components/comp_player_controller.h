#pragma once

#include "comp_base.h"
#include "geometry/transform.h"
#include "ia/ai_controller.h"

class TCompPlayerController : public IAIController {
  float   speedFactor = 8.0f;
  VEC3    speed;
  VEC3	  center;
  float	  radius;
	float   dashingMax;
	float   dashingAmount;
	int     dashingSpeed;
	bool    lookingLeft;

  DECL_SIBLING_ACCESS();

public:
  void debugInMenu();
  void load(const json& j, TEntityParseContext& ctx);

  // IA
  void IdleState();
  void RunningState();
  void JumpingState();
	void OmniDashingState();
	void DashingState();
	void DeadState();
	void MovePlayer(bool left);

  void Init();
};