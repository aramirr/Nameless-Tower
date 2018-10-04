#pragma once

#include "../comp_base.h"
#include "geometry/transform.h"


class TCompRenderWindstrike : public TCompBase {
  DECL_SIBLING_ACCESS();

  int id;

  bool carga = false;

public:
  void debugInMenu();
  void load(const json& j, TEntityParseContext& ctx);
  void update(float dt);

  void generate_windstrike_sprite();
};