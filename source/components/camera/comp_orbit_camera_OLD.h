#pragma once

#include "components/comp_base.h"
#include "camera/camera.h"

class TCompOrbitCamera : public TCompBase {

	CEntity *player;
	CEntity *platform;
	VEC3 towerCentre;
	VEC3 pos;

	VEC3 actualPos;

  float offset;
  //VEC3 playerOffset;
	float xOffset;

	float radio;

  float speedCaida;

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

	float playerY;
	float currentPlayerY;

  float getYSpeed();
	bool isForward();
  bool isGrounded();

	bool izquierda;
  bool caida;

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