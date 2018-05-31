#ifndef _AIC_TORCH
#define _AIC_TORCH

#include "../ai_controller.h"
#include <string>


class CAITorch : public IAIController
{
  DECL_SIBLING_ACCESS();

	float timer = 0.f;
	float timer_limit;
	bool in_puzzle;
    bool active = true;
    bool attached = false;
	std::string puzzle_name;

    bool b_fuego = true;
    bool on_start = true;

    // Light params
    VEC3 fire_position = VEC3::Zero;
    VEC4 color = VEC4(1, 1, 1, 1);
    float intensity = 1.0f;
    float radius = 1.0f;
    float initial_radius = 1.0f;
    bool b_increasing_radius = true;

public:
	
  void load(const json& j, TEntityParseContext& ctx);
  void debugInMenu();

  void Init();
  void ActiveState(float dt);
  void InactiveState(float dt);
  
  static void registerMsgs();
  void activate();
  void deactivate(const TMsgDeactivateTorch& msg);

  MAT44 getWorld();
  void renderDebug();
  void simulateLight();
};

#endif