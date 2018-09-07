#ifndef _AIC_PATROL
#define _AIC_PATROL

#include "ai_controller.h"

class CAICompresora : public IAIController
{
  std::string rol;
  std::string trigger1;
  std::string trigger2;
  std::string other_name;

  float speed = 2.0f;
  float wake_time = 0.0f;
  float delay = 1.0f;
  float acum_delay = 0.f;

  float maxY = 0.f;
  float minY = 0.f;
  bool going_up = false;

  bool player_in = false;

  void checkIfTouchingPlayer(const TMsgCheckPlayerIn& msg);
  void changeDirection(const TMsgChangeDirectionUp& msg);
  void playerIn(const TMsgPlayerIn& msg);
  void playerOut(const TMsgPlayerOut& msg);


  DECL_SIBLING_ACCESS();

public:
  void load(const json& j, TEntityParseContext& ctx);
  void debugInMenu();

  void Init();
  void MoveUpState();
  void MoveDownState();
  void WaitState();
  void SleepState();

  static void registerMsgs();

};

#endif