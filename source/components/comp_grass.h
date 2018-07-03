#pragma once

#include "comp_base.h"
#include "geometry/transform.h"


class TCompGrass : public TCompBase {
  DECL_SIBLING_ACCESS();

  float width = 0.f;
  float length = 0.f;
  int total = 100;

  VEC3 pos1 = VEC3::Zero;
  VEC3 pos2 = VEC3::Zero;

  bool carga = false;

public:
  void debugInMenu();
  void load(const json& j, TEntityParseContext& ctx);
  void update(float dt);

  void insert_grass();
};