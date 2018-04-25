#pragma once

#include "comp_base.h"
#include "geometry/transform.h"


class TCompPlayerKiller : public TCompBase {

  bool kill_enabled = true;

  void onTriggerEnter(const TMsgKillPlayer& msg);

  DECL_SIBLING_ACCESS();

public:
  static void registerMsgs();
  void debugInMenu();
  void load(const json& j, TEntityParseContext& ctx);
  void update(float dt);

  bool getKill();
  void setKill(bool new_value);
};