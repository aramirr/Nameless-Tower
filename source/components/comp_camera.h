#pragma once

#include "comp_base.h"
#include "camera/camera.h"

class TCompCamera : public CCamera, public TCompBase {

	CEntity *player;
	VEC3 towerCentre;
	VEC3 pos;

	//VEC3 front;
	//float dir;

	bool pForwarding; //guarda la direccion en la que estaba moviendose el player en el ultimo frame (True = para adelante || False = para atras)

	float X;
	float Y;
	float Z;

	float fov_deg;
	float z_near;
	float z_far;

	float distance;
	float height;

	bool isForward(VEC3 player, VEC3 frontPlayer);

public:
	void debugInMenu();
	void load(const json& j, TEntityParseContext& ctx);
	void update(float dt);
	DECL_SIBLING_ACCESS();
};