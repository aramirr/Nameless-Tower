#ifndef _AIC_TORCH_PUZZLE
#define _AIC_TORCH_PUZZLE

#include "../ai_controller.h"
#include "ai_torch.h"

class CAITorchPuzzle : public IAIController
{
  DECL_SIBLING_ACCESS();

	float timer = 0.f;
	float timer_limit;
	boolean active = false;
	std::vector<CAITorch*> torchs;


public:
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