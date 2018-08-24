#pragma once

#include "../comp_base.h"
#include "components/camera/comp_camera.h"

class CTexture;
class CRenderToTexture;

class TCompLightPoint : public TCompCamera {

  // Light params
  VEC4            color = VEC4(1, 1, 1, 1);
  float           intensity = 1.0f;
  float           radius = 1.0f;

	// Shadows params
	bool              shadows_enabled = false;    // Dynamic
	bool              casts_shadows = false;      // Static
	int               shadows_resolution = 256;
	float             shadows_step = 1.f;
	CRenderToCube* shadows_rt = nullptr;


public:
  void debugInMenu();
  void renderDebug();
  void load(const json& j, TEntityParseContext& ctx);
	void update(float dt);
  DECL_SIBLING_ACCESS();

	bool have_shadows() {
		if (shadows_rt)return true;
		return false;
	};
	void generateShadowMap();

  void activate(int i);
  MAT44 getWorld();
  void setIntensity(float value);
};