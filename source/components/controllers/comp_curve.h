#pragma once

#include "components/comp_base.h"

class CCurve;

class TCompCurve : public TCompBase
{
  DECL_SIBLING_ACCESS();

public:
  void debugInMenu();
  void load(const json& j, TEntityParseContext& ctx);
  void update(float dt);

private:
  std::string curve_name;
  const CCurve* _curve = nullptr;
  float _percentage = 0.f;
  float _speed = 0.f;
  CHandle _target;
  std::string _targetName;
  bool _automove = true;
  std::string curve_mode;
  int direction = 1;
  int current_mode = 0;
};

