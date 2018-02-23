#pragma once

#include "comp_base.h"
#include "camera/camera.h"

class TCompCameraManager : public TCompBase {

	CEntity *player;

	bool isForward(VEC3 player, VEC3 frontPlayer);
	bool pForwarding;

	bool carga;

public:
	void debugInMenu();
	void load(const json& j, TEntityParseContext& ctx);
	void update(float dt);
	DECL_SIBLING_ACCESS();
};