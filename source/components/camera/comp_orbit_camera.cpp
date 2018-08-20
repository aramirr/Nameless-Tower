#include "mcv_platform.h"
#include "comp_orbit_camera.h"
#include "components/juan/comp_transform.h"
#include "components/player/comp_player_controller.h"

DECL_OBJ_MANAGER("orbitCamera", TCompOrbitCamera);

bool TCompOrbitCamera::isForward() {
  TCompPlayerController* pc = player->get<TCompPlayerController>();
  return !pc->isForward();
}

bool TCompOrbitCamera::isGrounded() {
  TCompPlayerController* pc = player->get<TCompPlayerController>();
  return pc->isGrounded();

}

void TCompOrbitCamera::activateCamera(const TMsgActiveCamera & msg)
{
  active = true;
}

void TCompOrbitCamera::deactivateCamera(const TMsgDeactiveCamera & msg)
{
  active = false;
}

void TCompOrbitCamera::registerMsgs() {
  DECL_MSG(TCompOrbitCamera, TMsgActiveCamera, activateCamera);
  DECL_MSG(TCompOrbitCamera, TMsgDeactiveCamera, deactivateCamera);
}

void TCompOrbitCamera::debugInMenu() {
  ImGui::Text("Delante: %s", isForward() ? "Si" : "No");
  ImGui::Text("Suelo: %s", isGrounded() ? "Si" : "No");
  ImGui::Text("Active: %s", active ? "Si" : "No");
  ImGui::DragFloat("Distancia", &distance, 0.1f, -200.f, 200.f);
  ImGui::DragFloat("AP", &apertura, 0.1f, -2000.f, 2000.f);
  ImGui::DragFloat("Altura", &height, 0.1f, -20.f, 20.f);
  ImGui::DragFloat("Fov", &fov_deg, 0.1f, -1000.f, 1000.f);
  ImGui::DragFloat("Look_X", &X, 0.1f, -100.f, 100.f);
  ImGui::DragFloat("Look_Y", &Z, 0.1f, -100.f, 100.f);
  ImGui::DragFloat("Look_Z", &Y, 0.1f, -100.f, 100.f);
  ImGui::DragFloat("Radio", &radio, 0.1f, -100.f, 100.f);

}

void TCompOrbitCamera::load(const json& j, TEntityParseContext& ctx) {

  // ..
  player = (CEntity *)getEntityByName("The Player");

  fov_deg = j.value("fov", 75.f);
  z_near = j.value("z_near", 0.1f);
  z_far = j.value("z_far", 1000.f);
  distance = j.value("distance", 25.5f);
  height = j.value("height", 2.7f);
  radio = j.value("radio", 20.f);
  apertura = j.value("apertura", -278.f);

  X = 0;
  Y = 0;
  Z = 0;

  TCompTransform* p = player->get<TCompTransform>();
  assert(p);
  VEC3 pPos = p->getPosition();

  playerY = pPos.y;
  currentPlayerY = pPos.y;

  offset = deg2rad(((2 * 3.14159f * radio) / 360) * apertura);

  speedCaida = 0.f;
  pesoOldPosition = 0.75f;

  carga = true;
  active = true;

  camera = get<TCompCamera>();
}

void TCompOrbitCamera::update(float dt) {
  offset = deg2rad(((2 * 3.14159f * radio) / 360) * apertura);

  TCompTransform* c = get<TCompTransform>();
  assert(c);
  VEC3 pos = c->getPosition();

  VEC3 newPos;

  bool b_caida = false;

  TCompTransform* p = player->get<TCompTransform>();
  assert(p);
  VEC3 pPos = p->getPosition();

  float dY = abs(oldPos.y - pPos.y);

  if (currentPlayerY < pPos.y - 0.1f) {
	  //b_caida = false;
    if (dY > height + height + 1.5f) {
      currentPlayerY += 1.5f;
      speedCaida = 40.f;
    }
    else if (dY > height + (height / 2) + 1.5f) {
      currentPlayerY += 1.125f;
      speedCaida = 30.f;
    }
    else if (dY > height + (height / 3) + 1.5f) {
      currentPlayerY += 0.75f;
      speedCaida = 20.f;
    }
    else if (dY > height + (height / 4) + 1.5f) {
      currentPlayerY += 0.375f;
      speedCaida = 10.f;
    }
    else {
      currentPlayerY += 0.05f;
      speedCaida = 0.f;
    }
  }
  else if (currentPlayerY > pPos.y + 0.1f) {
    if (dY > height + height + 1.5f) speedCaida = 40.f;
    else if (dY > height + (height / 2) + 1.5f) speedCaida = 30.f;
    else if (dY > height + (height / 3) + 1.5f) speedCaida = 20.f;
    else if (dY > height + (height / 4) + 1.5f) speedCaida = 10.f;
    else speedCaida = 0.f;

	b_caida = true;
	currentPlayerY = pPos.y;


  }

  VEC3 center = VEC3(0 + X, currentPlayerY + height + Y, 0 + Z);

  if (active) {
    float d = VEC3::Distance(center, pPos);
    float _d = (d - distance) / d;
    float x = pPos.x - _d * (center.x - pPos.x);
    float z = pPos.z - _d * (center.z - pPos.z);

    pos.x = x;
    pos.y = currentPlayerY + height;
    pos.z = z;

    float _distance = VEC3::Distance(center, pos);

    if (isForward())offset *= -1;

    float y, p2, _y, _p2;
    c->getYawPitchRoll(&y, &p2);
    p->getYawPitchRoll(&_y, &_p2);

    c->setPosition(center);

    y = _y + offset;
    c->setYawPitchRoll(y, p2);
    newPos = c->getPosition() - (c->getFront() * _distance);
    newPos.y = currentPlayerY + height;
  }
  else {
    newPos = oldPos;
    newPos.y = currentPlayerY + height;
  }

  if (!carga) {
    actualPos = VEC3::Lerp(oldPos, newPos, dt *(10 + speedCaida));
    if (b_caida == true) {
      pesoOldPosition = 0.1f;
    }
    else {
      pesoOldPosition = 0.75f;
    }
	float pesoNewPosition = (1 - pesoOldPosition);
    //newPos = pesoOldPosition * oldPos + (1 - pesoOldPosition) * actualPos;
	newPos = VEC3(0.75f * oldPos.x, pesoOldPosition * oldPos.y, 0.75f * oldPos.z) + VEC3(0.25f * actualPos.x, pesoNewPosition * actualPos.y, 0.25f * actualPos.z);
  }
  else carga = false;

  oldPos = newPos;

  c->setPosition(newPos);
  c->lookAt(newPos, center);
}