#pragma once

#include "comp_base.h"
#include "geometry/transform.h"

class TCompCircularController : public TCompBase {

  float       radius = 1.0f;
  CHandle     h_target;
  std::string target_name;
  float       speed = 1.0f;
  float       curr_yaw = 0.f;

  DECL_SIBLING_ACCESS();

public:
  void debugInMenu();
  void load(const json& j, TEntityParseContext& ctx);
  void update(float dt);
};