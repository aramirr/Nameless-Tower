#pragma once

#include "comp_base.h"
#include "geometry/transform.h"


class TCompParticles : public TCompBase {

  DECL_SIBLING_ACCESS();

public:
  static void registerMsgs();
  void debugInMenu();
  void load(const json& j, TEntityParseContext& ctx);
  void update(float dt);
};