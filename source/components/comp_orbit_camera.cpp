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

bool TCompOrbitCamera::isGrounded()
{
  TCompPlayerController* pc = player->get<TCompPlayerController>();
  return pc->isGrounded();
}

//void TCompOrbitCamera::changeHeight(const TMsgisGrounded & msg)
//{
//	TCompTransform* p = player->get<TCompTransform>();
//	assert(p);
//	VEC3 pPos = p->getPosition();
//
//	playerY = pPos.y;
//}
//
//void TCompOrbitCamera::registerMsgs()
//{
//	DECL_MSG(TCompOrbitCamera, TMsgisGrounded, changeHeight);
//}

void TCompOrbitCamera::attachPlayer(const TMsgAttachTo & msg) {
  platform = (CEntity *)msg.h_attached;
  TCompTransform* pl = platform->get<TCompTransform>();
  assert(pl);
  VEC3 plPos = pl->getPosition();

  TCompTransform* p = player->get<TCompTransform>();
  assert(p);
  VEC3 pPos = p->getPosition();

  if (pPos.y > plPos.y)inPlatform = true;
}

void TCompOrbitCamera::detachPlayer(const TMsgDetachOf & msg) {
  if (inPlatform) {
    inPlatform = false;
    jumpinPlatform = true;
  }
}

void TCompOrbitCamera::exitFromPlatform(const TMsgExitPlatform & msg)
{
  exitPlatform = false;
}

void TCompOrbitCamera::registerMsgs() {
  DECL_MSG(TCompOrbitCamera, TMsgAttachTo, attachPlayer);
  DECL_MSG(TCompOrbitCamera, TMsgDetachOf, detachPlayer);
  DECL_MSG(TCompOrbitCamera, TMsgExitPlatform, exitFromPlatform);
}

void TCompOrbitCamera::debugInMenu() {
  ImGui::Text("inPlatform: %s", inPlatform ? "Si" : "No");
  ImGui::Text("jumpinPlatform: %s", jumpinPlatform ? "Si" : "No");
  ImGui::Text("exitPlatform: %s", exitPlatform ? "Si" : "No");
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
  ImGui::DragFloat("DISTCAM", &distanceCam, 0.1f, -100.f, 100.f);
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

  carga = true;

  inPlatform = false;
  jumpinPlatform = false;
  exitPlatform = false;
}

void TCompOrbitCamera::update(float dt) {
  xOffset = deg2rad(((2 * 3.14159f * radio) / 360) * apertura);
  TCompTransform* c = get<TCompTransform>();
  assert(c);
  pos = c->getPosition();
  actualPos = pos;

  TCompTransform* p = player->get<TCompTransform>();
  assert(p);
  VEC3 pPos = p->getPosition();

  VEC3 newPos;

  VEC2 pPos2D = VEC2(pPos.x, pPos.z);
  VEC2 pos2D = VEC2(pos.x, pos.z);

  distanceCam = VEC2::Distance(pPos2D, pos2D);

  bool izquierda = c->isInLeft(pPos);

  float dY = abs(currentPlayerY - pPos.y);

  if (currentPlayerY < pPos.y - 0.1f) {
    if (dY > 10.f)currentPlayerY += 2.05f;//12.5f;
    else if (dY > 7.f)currentPlayerY += 1.05f;//6.5f;
    else if (dY > 4.f)currentPlayerY += 0.05f;
    else currentPlayerY += 0.025f;
  }
  if (currentPlayerY > pPos.y + 0.1f) {
    if (dY > 10.f)currentPlayerY -= 2.05f; //12.5f;
    else if (dY > 7.f)currentPlayerY -= 1.05f; //6.5f;
    else if (dY > 4.f)currentPlayerY -= 0.05f;
    else currentPlayerY -= 0.025f;
  }

  VEC3 center = VEC3(0 + X, currentPlayerY + height + Y, 0 + Z);

  VEC3 center2 = center;
  center2.y = pPos.y + height + Y;

  float distanceT = VEC3::Distance(center2, pPos);

  //dbg("%f\n", (distance - distanceT));

//  if (inPlatform) {
//    dbg("AAAAAAAAAA\n");
//  }

  if (jumpinPlatform || (exitPlatform && !isGrounded()) || (izq && !isForward() && !inPlatform || ((izq && isForward()) && (distanceCam > (abs(distance - distanceT) + 0.25f)) && (distanceCam < 9.f) && izquierda && !inPlatform))
    || (!izq && isForward() && !inPlatform || ((!izq && !isForward()) && (distanceCam > (abs(distance - distanceT) + 0.25f)) && (distanceCam < 9.f) && !izquierda && !inPlatform))) {
    newPos = pos;
    newPos.y = currentPlayerY + height;
    //if (exitPlatform && isGrounded()) {
    //  // dbg("Saaaaaaaaaaaaaaalgoooooooooooooooooo\n");
    //  //newPos = actualPos;
    //  //VEC3 newPos2 = ((newPos - actualPos) / 200.f);
    //  //newPos.x = actualPos.x + newPos2.x;
    //  //newPos.z = actualPos.z + newPos2.z;
    //  exitPlatform = false;
    //}
    if (jumpinPlatform) {
     // dY = abs(currentPlayerY - pPos.y);
      //if (/*inPlatform || (dY > 4.f && */isGrounded()/*)*/) {
        jumpinPlatform = false;
        exitPlatform = true;
      //}
      //else if(inOrbitPlatform || isGrounded()) jumpinOrbitPlatform = false;
      /*if (isGrounded()) {
        jumpinOrbitPlatform = false;
      }*/
    }
  }
  else {
    /*if (inOrbitPlatform && ((izq && isForward()) || (!izq && !isForward()))) {
      TMsgchangeCamerainPlatform msg;
      CEntity* camManager = (CEntity *)getEntityByName("camera_manager");
      camManager->sendMsg(msg);
    }*/
    /*else {*/
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


    if (!inPlatform)y = _y + xOffset;
    else if (((izq && isForward()) || (!izq && !isForward())))y = _y + xOffset;

    c->setYawPitchRoll(y, p2);
    newPos = c->getPosition() - (c->getFront() * (_distance - distance));
    newPos.y = currentPlayerY + height;

    if ((exitPlatform /*&& isGrounded()*/) || inPlatform) {
     // dbg("Saaaaaaaaaaaaaaalgoooooooooooooooooo\n");
      //newPos = actualPos;
      VEC2 newPos2D = VEC2(newPos.x, newPos.z);
      VEC2 actualPos2D = VEC2(actualPos.x, actualPos.z);
      float dist = VEC2::Distance(newPos2D, actualPos2D);
      //dbg("%f\n", dist);
      if (dist <= 0.25 && exitPlatform && isGrounded()) {
        exitPlatform = false;

        TMsgExitPlatform msg;
        if (izq) {
          CEntity* camDER = (CEntity *)getEntityByName("camera_orbit_DER");
          camDER->sendMsg(msg);
        }
        else {
          CEntity* camIZQ = (CEntity *)getEntityByName("camera_orbit_IZQ");
          camIZQ->sendMsg(msg);
        }
      }
      else {
        float div = 35.f;
        pPos = p->getPosition();
        VEC2 pPos2D = VEC2(pPos.x, pPos.z);
        if (VEC2::Distance(newPos2D, pPos2D) > 1.5f)div = 25.f;
        dbg("%f\n", (newPos - pPos));
        VEC3 newPos2 = ((newPos - actualPos) / div);
        newPos.x = actualPos.x + newPos2.x;
        newPos.z = actualPos.z + newPos2.z;
      }
    }
  }

  //float distanceBC = VEC3::Distance(newPos, actualPos);
  //if (inPlatform /*&& distanceBC > 3.f*/) {
  //  /*newPos = ((newPos - actualPos) / 200.f);
  //  newPos += actualPos;
  //  newPos.y = currentPlayerY + height;*/
  //  c->setPosition(newPos);
  //}
  //else {
    c->setPosition(newPos);
  /*}*/

  //c->setPerspective(deg2rad(fov_deg), z_near, z_far);
  c->lookAt(newPos, center);
  /*}*/
}

