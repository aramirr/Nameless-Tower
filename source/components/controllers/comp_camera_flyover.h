#pragma once

#include "components/comp_base.h"

class TCompCameraFlyover : public TCompBase
{
  DECL_SIBLING_ACCESS();

public:
  void debugInMenu();
  void load(const json& j, TEntityParseContext& ctx);
  void update(float dt);

private:
  float _speed = 10.f;
  float _sensitivity = 0.005f;
  float _maxPitch = (float)M_PI_2 - 1e-4f;
  VEC3  _ispeed;
  float _ispeed_reduction_factor = 0.15f;
  bool  _enabled = true;
  int   _key_toggle_enabled = 0;
	bool activo = true;
};

