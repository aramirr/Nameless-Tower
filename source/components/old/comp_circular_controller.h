#pragma once

#include "components/comp_base.h"
#include "geometry/transform.h"
#include "entity/common_msgs.h"

class TCompCircularController : public TCompBase {

  float       speed = 1.0f;
  float       curr_rad = 0.f;
  char        eje = 'X';

  VEC3		    u;

  bool carga;

  DECL_SIBLING_ACCESS();

public:
  void debugInMenu();
  void load(const json& j, TEntityParseContext& ctx);
  void update(float dt);
};