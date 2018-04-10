#pragma once

#include "components/comp_base.h"
#include "camera/camera.h"

class TCompCamera : public CCamera, public TCompBase {
	
	CEntity *player;
	VEC3 pos;

	float height;

public:

	void debugInMenu();
	void renderDebug();
	void load(const json& j, TEntityParseContext& ctx);
	void update(float dt);
	DECL_SIBLING_ACCESS();
};