#pragma once

#include "components/comp_base.h"

class CCurve;

class TCompCurve : public TCompBase
{
  DECL_SIBLING_ACCESS();

public:
  void debugInMenu();
  void load(const json& j, TEntityParseContext& ctx);
  void update(float DT);
  void setRatio(float ratio) { _percentage = ratio; }
	void activate();
	void deactivate();
private:
  VEC3 getTargetPos();
  const CCurve* _curve = nullptr;
  float _percentage = 0.f;
  float _speed = 0.f;
  CHandle _target;
  std::string _targetName;
  bool _automove = true;
  std::string curve_mode;
  int direction = 1;
  int current_mode = 0;
  bool loop = true;
};

