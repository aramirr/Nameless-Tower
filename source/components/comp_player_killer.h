#pragma once

#include "comp_base.h"
#include "geometry/transform.h"


class TCompPlayerKiller : public TCompBase {

  bool kill = true;

  void onTriggerEnter();

  DECL_SIBLING_ACCESS();

public:
  static void registerMsgs();
  void debugInMenu();
  void load(const json& j, TEntityParseContext& ctx);
  void update(float dt);

  bool getKill();
  void setKill(bool new_value);
};