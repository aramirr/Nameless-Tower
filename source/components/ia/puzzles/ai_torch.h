#ifndef _AIC_TORCH
#define _AIC_TORCH

#include "../ai_controller.h"
#include <string>


class CAITorch : public IAIController
{
  DECL_SIBLING_ACCESS();

	float timer = 0.f;
    float delay;
  float frames;
  float smoke_frames;
  float current_frames;
  float y_offset = 0.f;
  float smoke_y_offset = 0.f;
  float x_offset = 0.f;
  float z_offset = 0.f;
  bool apagando = false;
	bool apagado = false;
	bool initialized = false;
	float timer_limit;
	float current_delay;
    bool in_puzzle;
    bool violeta;
    bool azul;
    bool active = true;
    bool render;
    bool attached = false;
	std::string puzzle_name;

    bool b_fuego = true;
    bool on_start = true;
    bool thin = true;

    // Light params
    VEC3 fire_position = VEC3::Zero;
	VEC4 color = VEC4(1, 1, 1, 1);
    float intensity = 1.0f;
    float radius = 1.0f;
	float scale = 1.f;
	VEC4 initial_color = VEC4(1, 1, 1, 1);
    float initial_radius = 1.0f;
    bool b_increasing_radius = true;
	int id = -1;

public:
	
  void load(const json& j, TEntityParseContext& ctx);
  void debugInMenu();

  void Init();
  void ActiveState(float dt);
  void InactiveState(float dt);
  
  static void registerMsgs();
  void activate();
  void deactivate(const TMsgDeactivateTorch& msg);
  void activateMsg(const TMsgActivateTorch& msg);

  MAT44 getWorld();
  void renderDebug();
  void simulateLight();
};

#endif