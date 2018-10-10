#include "mcv_platform.h"
#include "comp_platform_camera.h"
#include "components/juan/comp_transform.h"
#include "components/player/comp_player_controller.h"
#include <iostream>

DECL_OBJ_MANAGER("platformCamera", TCompPlatformCamera);

bool TCompPlatformCamera::isForward()
{
	TCompPlayerController* pc = player->get<TCompPlayerController>();
	return !pc->isForward();
}

bool TCompPlatformCamera::isGrounded()
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

void TCompPlatformCamera::attachPlayer(const TMsgAttachTo & msg) {
	platform = (CEntity *)msg.h_attached;
	TCompTransform* pl = platform->get<TCompTransform>();
	assert(pl);
	VEC3 plPos = pl->getPosition();

	TCompTransform* p = player->get<TCompTransform>();
	assert(p);
	VEC3 pPos = p->getPosition();

	if (pPos.y > plPos.y)inPlatform = true;
}

void TCompPlatformCamera::detachPlayer(const TMsgDetachOf & msg) {
	if (inPlatform) {
		inPlatform = false;
		jumpinPlatform = true;
	}
}

void TCompPlatformCamera::exitFromPlatform(const TMsgExitPlatform & msg)
{
	exitPlatform = false;
}

void TCompPlatformCamera::registerMsgs() {
	DECL_MSG(TCompPlatformCamera, TMsgAttachTo, attachPlayer);
	DECL_MSG(TCompPlatformCamera, TMsgDetachOf, detachPlayer);
	DECL_MSG(TCompPlatformCamera, TMsgExitPlatform, exitFromPlatform);
}

void TCompPlatformCamera::debugInMenu() {
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

void TCompPlatformCamera::load(const json& j, TEntityParseContext& ctx) {

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

	speedCaida = 0.f;
}

VEC3 oldpos2;

void TCompPlatformCamera::update(float dt) {
	xOffset = deg2rad(((2 * 3.14159f * radio) / 360) * apertura);
	TCompTransform* c = get<TCompTransform>();
	assert(c);
	pos = c->getPosition();

	TCompTransform* p = player->get<TCompTransform>();
	assert(p);
	VEC3 pPos = p->getPosition();

	VEC3 newPos;

	VEC2 pPos2D = VEC2(pPos.x, pPos.z);
	VEC2 pos2D = VEC2(pos.x, pos.z);

	distanceCam = VEC2::Distance(pPos2D, pos2D);

	bool izquierda = c->isInLeft(pPos);

	float dY = abs(actualPos.y - pPos.y);

	if (currentPlayerY < pPos.y - 0.1f) {
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
			speedCaida = 10.f;
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
		if (dY > height + height + 1.5f) {
			currentPlayerY -= 1.5f;
			speedCaida = 40.f;
		}
		else if (dY > height + (height / 2) + 1.5f) {
			currentPlayerY -= 1.125f;
			speedCaida = 30.f;
		}
		else if (dY > height + (height / 3) + 1.5f) {
			currentPlayerY -= 0.75f;
			speedCaida = 20.f;
		}
		else if (dY > height + (height / 4) + 1.5f) {
			currentPlayerY -= 0.375f;
			speedCaida = 10.f;
		}
		else {
			currentPlayerY -= 0.05f;
			speedCaida = 0.f;
		}
	}

	VEC3 center = VEC3(0 + X, currentPlayerY + height + Y, 0 + Z);

	VEC3 center2 = center;
	center2.y = pPos.y + height + Y;

	float distanceT = VEC3::Distance(center2, pPos);

	float d = VEC3::Distance(center, pPos);
	float _d = (d - distance) / d;
	float x = pPos.x - _d * (center.x - pPos.x);
	float z = pPos.z - _d * (center.z - pPos.z);

	pos.x = x;
	pos.y = currentPlayerY + height;
	pos.z = z;

	newPos = pos;
	
  VEC3 desiredpos;

  desiredpos = VEC3::Lerp(actualPos, newPos, dt * (10 + speedCaida)  /*(10 + speedCaida * dt)*/ /** getYSpeed()*/);//(10 + speedCaida)/*chaseSpeed + cameraCatchup*/);

  float vel = 0.75;

  newPos = vel * oldpos2 + (1 - vel)*desiredpos;
  oldpos2 = newPos;
	c->setPosition(newPos);
	actualPos = newPos;


  VEC3 aim = center;
  aim = aim + p->getFront()*0;
	c->lookAt(newPos, aim);
}

