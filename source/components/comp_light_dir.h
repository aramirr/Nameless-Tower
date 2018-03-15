#pragma once

#include "comp_base.h"
#include "comp_camera.h"

class CTexture;

class TCompLightDir : public TCompCamera {

  VEC4            color = VEC4(1, 1, 1, 1);
  float           intensity = 1.0f;
  const CTexture* projector = nullptr;

public:
  void debugInMenu();
  void renderDebug();
  void load(const json& j, TEntityParseContext& ctx);
  void update(float dt);
  DECL_SIBLING_ACCESS();

  void activate();

};