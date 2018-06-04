#pragma once

#include "components/comp_base.h"
#include "camera/camera.h"

class TCompOrbitCamera : public TCompBase {

	CEntity *player;
	VEC3 towerCentre;

	VEC3 actualPos;
  VEC3 oldPos;

  float offset;
	float radio;

  float speedCaida;
  float vel;

	float X;
	float Y;
	float Z;

	float fov_deg;
	float z_near;
	float z_far;

	float distance;
	float height;

	float apertura;

	float playerY;
	float currentPlayerY;

	bool isForward();
  bool isGrounded();

	void activeCamera(const TMsgActiveCamera& msg);
	void desactiveCamera(const TMsgDesactiveCamera& msg);

	bool active;
  bool carga;

public:
	static void registerMsgs();

	void debugInMenu();
	void load(const json& j, TEntityParseContext& ctx);
	void update(float dt);
	DECL_SIBLING_ACCESS();
};