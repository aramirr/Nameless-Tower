#pragma once

#include "components/comp_base.h"
#include "camera/camera.h"
#include "comp_camera.h"

class TCompOrbitCamera : public TCompBase {

  TCompCamera* camera;

  CEntity *player;
  VEC3 towerCentre;

  VEC3 actualPos;
  VEC3 oldPos;

  float offset;
  float radio;

  float speedCaida;
  float pesoOldPosition;

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

  bool carga;

  bool active;

  void activateCamera(const TMsgActiveCamera& msg);
  void deactivateCamera(const TMsgDeactiveCamera& msg);

public:
  static void registerMsgs();

  void debugInMenu();
  void load(const json& j, TEntityParseContext& ctx);
  void update(float dt);
	void setPosition(VEC3 position);
  DECL_SIBLING_ACCESS();
};