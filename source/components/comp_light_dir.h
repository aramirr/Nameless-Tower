#pragma once

#include "comp_base.h"
#include "components/camera/comp_camera.h"

class CTexture;
class CRenderToTexture;

class TCompLightDir : public TCompCamera {

	// Light params
	VEC4            color = VEC4(1, 1, 1, 1);
	float           intensity = 1.0f;
	const CTexture* projector = nullptr;

	// Shadows params
	bool              shadows_enabled = false;    // Dynamic
	bool              casts_shadows = false;      // Static
	int               shadows_resolution = 256;
	float             shadows_step = 1.f;
	CRenderToTexture* shadows_rt = nullptr;

public:
	void debugInMenu();
	void renderDebug();
	void load(const json& j, TEntityParseContext& ctx);
	void update(float dt);
	DECL_SIBLING_ACCESS();

	void activate();
	void generateShadowMap();

};