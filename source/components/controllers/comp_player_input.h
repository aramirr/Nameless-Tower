#pragma once

#include "components/comp_base.h"


class TCompPlayerInput : public TCompBase
{
  DECL_SIBLING_ACCESS();

public:
  void debugInMenu();
  void load(const json& j, TEntityParseContext& ctx);
  void update(float dt);
  float windstrike_time = 1;
  int windstrike_wait_time = 1;

private:
};

