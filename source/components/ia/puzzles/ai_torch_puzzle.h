#ifndef _AIC_TORCH
#define _AIC_TORCH

#include "../ai_controller.h"
#include "ai_torch.h"

class CAITorchPuzzle : public IAIController
{
  DECL_SIBLING_ACCESS();

	float timer = 0.f;
	float timer_limit;
	boolean active;
	std::vector<CAITorch> config_states;


public:
	boolean active = false;
  void load(const json& j, TEntityParseContext& ctx);
  void debugInMenu();

  void Init();
	void ActiveState(float dt);
	void InactiveState(float dt);
  
  static void registerMsgs();
	void deactivate();
	void activate(const TMsgActivateTorchPuzzle& msg);
};

#endif