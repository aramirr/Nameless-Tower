#pragma once

#include "comp_base.h"
#include "camera/camera.h"

class TCompCameraManager : public TCompBase {

	CEntity *player;

	bool isForward();
	bool pForwarding;

	bool carga;

  //void changeCamerainPlatform(const TMsgchangeCamerainPlatform& msg);

  bool inPlatform;

public:
  //static void registerMsgs();

	void debugInMenu();
	void load(const json& j, TEntityParseContext& ctx);
	void update(float dt);
	DECL_SIBLING_ACCESS();
};