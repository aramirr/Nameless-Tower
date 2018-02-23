#pragma once

#include "comp_base.h"
#include "camera/camera.h"

class TCompOrbitCamera : public TCompBase {

	CEntity *player;
	VEC3 towerCentre;
	VEC3 pos;

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

  void changeHeight(const TMsgisGrounded& msg);

  float playerY;
  float currentPlayerY;

public:
  static void registerMsgs();

	void debugInMenu();
	void load(const json& j, TEntityParseContext& ctx);
	void update(float dt);
	DECL_SIBLING_ACCESS();
};