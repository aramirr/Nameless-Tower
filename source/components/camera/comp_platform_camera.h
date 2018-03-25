#pragma once

#include "components/comp_base.h"
#include "camera/camera.h"

class TCompPlatformCamera : public TCompBase {

	CEntity *player;
  CEntity *platform;
	VEC3 towerCentre;
	VEC3 pos;

	VEC3 actualPos;

	float xOffset;

	float radio;

	float X;
	float Y;
	float Z;

	float fov_deg;
	float z_near;
	float z_far;

	float distance;
	float height;

	float apertura;

	bool izq;

  bool isForward();
	bool isGrounded();

  //void changeHeight(const TMsgisGrounded& msg);

  float distanceCam;

  float playerY;
  float currentPlayerY;

  bool carga;

  void attachPlayer(const TMsgAttachTo& msg);
  void detachPlayer(const TMsgDetachOf& msg);
  void exitFromPlatform(const TMsgExitPlatform& msg);

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