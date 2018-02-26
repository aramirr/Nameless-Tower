#include "mcv_platform.h"
#include "comp_orbit_camera.h"
#include "comp_transform.h"
#include "comp_player_controller.h"
#include <iostream>

DECL_OBJ_MANAGER("orbitCamera", TCompOrbitCamera);

bool TCompOrbitCamera::isForward()
{
  TCompPlayerController* pc = player->get<TCompPlayerController>();
  return !pc->isForward();
}

void TCompOrbitCamera::changeHeight(const TMsgisGrounded & msg)
{
  TCompTransform* p = player->get<TCompTransform>();
  assert(p);
  VEC3 pPos = p->getPosition();

  playerY = pPos.y;
}

void TCompOrbitCamera::registerMsgs()
{
  DECL_MSG(TCompOrbitCamera, TMsgisGrounded, changeHeight);
}

void TCompOrbitCamera::debugInMenu() {
	ImGui::DragFloat("Distancia", &distance, 0.1f, -200.f, 200.f);
	ImGui::DragFloat("AP", &distanceCam, 0.1f, -2000.f, 2000.f);
	ImGui::DragFloat("OS", &distance, 0.1f, -2000.f, 2000.f);
	ImGui::DragFloat("Altura", &height, 0.1f, -20.f, 20.f);
	ImGui::DragFloat("Fov", &fov_deg, 0.1f, -1000.f, 1000.f);
	ImGui::DragFloat("Look_X", &X, 0.1f, -100.f, 100.f);
	ImGui::DragFloat("Look_Y", &Z, 0.1f, -100.f, 100.f);
	ImGui::DragFloat("Look_Z", &Y, 0.1f, -100.f, 100.f);
}

void TCompOrbitCamera::load(const json& j, TEntityParseContext& ctx) {

	// ..
	player = (CEntity *)getEntityByName("The Player");

	fov_deg = j.value("fov", 75.f);
	z_near = j.value("z_near", 0.1f);
	z_far = j.value("z_far", 1000.f);
	distance = j.value("distance", 7.7f);
	height = j.value("height", 2.7f);
	radio = j.value("radio", 15.f);
	izq = j.value("izq", true);

	X = 0;
	Y = 0;
	Z = 0;
     
	apertura = -360.f;

  TCompTransform* p = player->get<TCompTransform>();
  assert(p);
  VEC3 pPos = p->getPosition();

  playerY = pPos.y;
  currentPlayerY = pPos.y;

	xOffset = deg2rad(((2 * 3.14159 * radio) / 360) * apertura);

  carga = true;
}

void TCompOrbitCamera::update(float dt) {
	xOffset = deg2rad(((2 * 3.14159 * radio) / 360) * apertura);
	TCompTransform* c = get<TCompTransform>();
	assert(c);
	pos = c->getPosition();

	TCompTransform* p = player->get<TCompTransform>();
	assert(p);
	VEC3 pPos = p->getPosition();

  VEC3 center = VEC3(0 + X, currentPlayerY + height + Y, 0 + Z);

  VEC3 newPos;

  /*VEC3 pPos2 = pPos;
  pPos2.y += height;*/

  distanceCam = VEC3::Distance(pPos, pos);

  if (abs(distanceCam - distance) <= 0.3)carga = false;

  if (!carga)dbg("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n");

  if (currentPlayerY < playerY)currentPlayerY += 0.02f;
  if (currentPlayerY > playerY)currentPlayerY -= 0.02f;

  if ((izq && !isForward() || (izq && isForward() && distanceCam > distance && distanceCam < 9.f))
    || (!izq && isForward() || (!izq && !isForward() && distanceCam > distance && distanceCam < 9.f))) {
    newPos = pos;
  }
  else {
    if (izq)xOffset *= -1;

    float d = VEC3::Distance(center, pPos);
    float _d = d / d;
    float x = pPos.x - _d * (center.x - pPos.x);
    float z = pPos.z - _d * (center.z - pPos.z);

    pos.x = x;
    pos.y = currentPlayerY + height;
    pos.z = z;

    float _distance = VEC3::Distance(center, pos);

    float y, p2, _y, _p2;
    c->getYawPitchRoll(&y, &p2);
    p->getYawPitchRoll(&_y, &_p2);

    c->setPosition(center);

    y = _y + xOffset;

    c->setYawPitchRoll(y, p2);
    newPos = c->getPosition() - (c->getFront() * (_distance - distance));
    newPos.y = currentPlayerY + height;
  }

	c->setPosition(newPos);

	//c->setPerspective(deg2rad(fov_deg), z_near, z_far);
	c->lookAt(newPos, center);
}

