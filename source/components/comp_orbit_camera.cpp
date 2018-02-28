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
}

void TCompOrbitCamera::update(float dt) {
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

	float dY = abs(currentPlayerY - pPos.y);

	if (currentPlayerY < pPos.y) {
		if (dY > 10.f)currentPlayerY += 5.f;
		else if(dY > 7.f)currentPlayerY += 0.5f;
		else if(dY > 4.f)currentPlayerY += 0.05f;
		else currentPlayerY += 0.01f;
	}
	if (currentPlayerY > pPos.y) {
		if (dY > 10.f)currentPlayerY -= 5.f;
		else if (dY > 7.f)currentPlayerY -= 0.5f;
		else if (dY > 4.f)currentPlayerY -= 0.05f;
		else currentPlayerY -= 0.01f;
	}

	VEC3 center = VEC3(0 + X, currentPlayerY + height + Y, 0 + Z);

	VEC3 center2 = center;
	center2.y = pPos.y + height + Y;

	float distanceT = VEC3::Distance(center2, pPos);

	dbg("%f\n", (distance - distanceT));

	if ((izq && !isForward() || ((izq && isForward()) && (distanceCam > (abs(distance - distanceT) + 0.25f)) && (distanceCam < 9.f) && izquierda))
		|| (!izq && isForward() || ((!izq && !isForward()) && (distanceCam >(abs(distance - distanceT) + 0.25f)) && (distanceCam < 9.f) && !izquierda))) {
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

