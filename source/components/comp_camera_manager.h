#pragma once

#include "comp_base.h"
#include "camera/camera.h"

class TCompCameraManager : public TCompBase {

	CEntity *player;
	CEntity *platform;

	bool isGrounded();
	bool isForward();
	bool pForwarding;

	bool carga;

	bool on;

	//void changeCamerainPlatform(const TMsgchangeCamerainPlatform& msg);

	bool lateral;

	void attachPlayer(const TMsgAttachTo& msg);
	void detachPlayer(const TMsgDetachOf& msg);

	void apagarManager(const TMsgCinematicON& msg);
	void encenderManager(const TMsgCinematicOFF& msg);

	bool inPlatform;
	bool jumpinPlatform;
	bool exitPlatform;

public:
	static void registerMsgs();

	void debugInMenu();
	void load(const json& j, TEntityParseContext& ctx);
	void update(float dt);
	DECL_SIBLING_ACCESS();
};