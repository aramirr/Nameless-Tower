#include "mcv_platform.h"
#include "comp_orbit_camera.h"
#include "components/juan/comp_transform.h"
#include "components/player/comp_player_controller.h"
#include <iostream>

DECL_OBJ_MANAGER("orbitCamera", TCompOrbitCamera);

bool TCompOrbitCamera::isForward() {
  TCompPlayerController* pc = player->get<TCompPlayerController>();
  return !pc->isForward();
}

bool TCompOrbitCamera::isGrounded() {
  TCompPlayerController* pc = player->get<TCompPlayerController>();
  return pc->isGrounded();
}

void TCompOrbitCamera::activeCamera(const TMsgActiveCamera & msg) {
  active = true;
}

void TCompOrbitCamera::desactiveCamera(const TMsgDesactiveCamera & msg) {
  active = false;
}

void TCompOrbitCamera::registerMsgs() {
  DECL_MSG(TCompOrbitCamera, TMsgActiveCamera, activeCamera);
  DECL_MSG(TCompOrbitCamera, TMsgDesactiveCamera, desactiveCamera);
}

void TCompOrbitCamera::debugInMenu() {
  ImGui::Text("Izquierda: %s", izquierda ? "Si" : "No");
  ImGui::Text("Delante: %s", isForward() ? "Si" : "No");
  ImGui::Text("Suelo: %s", isGrounded() ? "Si" : "No");
  ImGui::DragFloat("Distancia", &distance, 0.1f, -200.f, 200.f);
  ImGui::DragFloat("AP", &apertura, 0.1f, -2000.f, 2000.f);
  ImGui::DragFloat("OS", &xOffset, 0.1f, -2000.f, 2000.f);
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
  izq = j.value("izq", true);

  X = 0;
  Y = 0;
  Z = 0;

  apertura = -268.f;

  TCompTransform* p = player->get<TCompTransform>();
  assert(p);
  VEC3 pPos = p->getPosition();

  playerY = pPos.y;
  currentPlayerY = pPos.y;

  xOffset = deg2rad(((2 * 3.14159f * radio) / 360) * apertura);

  izquierda = false;

  if (izq) offset = -2.2;
  else offset = 2.2;
  chaseSpeed = 0.05f;

  caida = false;

  carga = true;
}

void TCompOrbitCamera::update(float dt) {
  xOffset = deg2rad(((2 * 3.14159f * radio) / 360) * apertura);
  TCompTransform* c = get<TCompTransform>();
  assert(c);
  pos = c->getPosition();

  TCompTransform* p = player->get<TCompTransform>();
  assert(p);
  VEC3 pPos = p->getPosition();

  /*std::string strPos = std::to_string(pPos.x) + " - " + std::to_string(pPos.y) + " - " + std::to_string(pPos.z) + "\n";
  dbg(strPos.c_str());
  if (izq) {
  dbg("izq\n");
  }
  else {
  dbg("der\n");
  }
  dbg("-------------------------------------------\n");*/

  /* if (carga) {
     playerOffset = pos - pPos;

     carga = false;
   }*/

  VEC3 newPos;

  float dY = abs(actualPos.y - pPos.y);

  //dbg((std::to_string(dY) + " == " + std::to_string(height) + "\n").c_str());
  if (caida && isGrounded()) {
    currentPlayerY = pPos.y;
    caida = false;
  }

  if (currentPlayerY < pPos.y - 0.1f) {
    //dbg((std::to_string(dY) + " > " + std::to_string(height) + "\n").c_str());
    if (dY > height + 0.7f) {
      currentPlayerY = pPos.y + height;//12.5f;
      caida = true;
      //Y = 2.f;
    }
    //else if (dY > 7.f)currentPlayerY += 11.f;//6.5f;
    //else if (dY > 4.f)currentPlayerY += 8.f;
    else currentPlayerY += 0.025f;
  }
  if (currentPlayerY > pPos.y + 0.1f) {
    dbg((std::to_string(dY) + " > " + std::to_string(height) + "\n").c_str());
    if (dY > height + 0.7f) {
      currentPlayerY = pPos.y - height; //12.5f;
      caida = true;
      //Y = 2.f;
    }
    //else if (dY > 7.f)currentPlayerY -= 11.f; //6.5f;
    //else if (dY > 4.f)currentPlayerY -= 8.f;
    else currentPlayerY -= 0.025f;
  }

  VEC3 center = VEC3(0 + X, currentPlayerY + height + Y, 0 + Z);

  //izquierda = c->isInLeft(pPos);

  VEC2 pPos2D = VEC2(pPos.x, pPos.z);
  VEC2 pos2D = VEC2(pos.x, pos.z);

  float distanceCam = VEC2::Distance(pPos2D, pos2D);

  VEC3 center2D = VEC2(center.x, center.z);;

  float distanceT = VEC3::Distance(center2D, pPos);
  distanceT = abs(distance - distanceT);

  bool delante = isForward();

  izquierda = c->isInLeft(pPos);

  if ((izq && !delante || ((izq && delante) && (distanceCam > distanceT) && (distanceCam < 9.f) && izquierda))        //CAMARA IZQ
    || (!izq && delante || ((!izq && !delante) && (distanceCam > distanceT) && (distanceCam < 9.f) && !izquierda))) { //CAMARA DER
    newPos = pos;
    newPos.y = currentPlayerY + height;
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

    if (VEC3::Distance(actualPos, center) < distance)carga = true;

    /*std::string strPos = std::to_string(VEC3::Distance(actualPos, pPos)) + " - " + std::to_string(distance) + "\n";
    dbg(strPos.c_str());
    if (izq) {
      dbg("izq\n");
    }
    else {
      dbg("der\n");
    }
    dbg("-------------------------------------------\n");*/

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
  /*if (VEC3::Distance(newPos, actualPos) > 1.f) {
    c->setPosition(newPos);
    actualPos = newPos;
    c->lookAt(newPos, center);
  }
  else {
    newPos = actualPos + (newPos - actualPos);
    c->setPosition(newPos);
    actualPos = newPos;
    c->lookAt(newPos, center);
  }*/
  if (carga) {
    carga = false;
    
  }
  else {
    
    float dP = VEC3::Distance(actualPos, pPos);

    if (dP > 10.f) chaseSpeed = 0.7f;
    else if (dP > 7.f) chaseSpeed = 0.4f;
    else if (dP > 4.f) chaseSpeed = 0.1f;
    else chaseSpeed = 0.05f;

    newPos = actualPos + (newPos - actualPos) * chaseSpeed;
  }

  /*std::string strPos = std::to_string(newPos.x) + " - " + std::to_string(newPos.y) + " - " + std::to_string(newPos.z) + "\n";
  dbg(strPos.c_str());
  if (izq) {
    dbg("izq\n");
  }
  else {
    dbg("der\n");
  }
  dbg("-------------------------------------------\n");*/

  c->setPosition(newPos);
  actualPos = newPos;
  c->lookAt(newPos, center);

}

