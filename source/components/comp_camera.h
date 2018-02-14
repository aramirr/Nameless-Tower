#pragma once

#include "comp_base.h"
#include "camera/camera.h"

class TCompCamera : public CCamera, public TCompBase {

	CEntity *player;
	VEC3 towerCentre;
	VEC3 pos;

	float X;
	float Y;
	float Z;

	float fov_deg;
	float z_near;
	float z_far;

	float dist = 10.f;
	float h = 5.f;

public:
  void debugInMenu();
  void load(const json& j, TEntityParseContext& ctx);
  void update(float dt);
  DECL_SIBLING_ACCESS();
};