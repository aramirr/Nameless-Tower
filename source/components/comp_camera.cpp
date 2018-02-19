#include "mcv_platform.h"
#include "comp_camera.h"
#include "comp_transform.h"
#include <iostream>

DECL_OBJ_MANAGER("camera", TCompCamera);

bool TCompCamera::isForward(VEC3 player, VEC3 frontPlayer) {
	if (player.x > 0 && player.z < 0) {								// CUADRANTE 1 (+-)
		if (frontPlayer.x < 0 && frontPlayer.z < 0) return true;
		return false;
	}
	else if (player.x < 0 && player.z < 0) {						// CUADRANTE 2 (--)
		if (frontPlayer.x < 0 && frontPlayer.z > 0) return true;
		return false;
	}
	else if (player.x < 0 && player.z > 0) {						// CUADRANTE 3 (-+)
		if (frontPlayer.x > 0 && frontPlayer.z > 0) return true;
		return false;
	}
	else if (player.x > 0 && player.z > 0) {						// CUADRANTE 4 (++)
		if (frontPlayer.x > 0 && frontPlayer.z < 0) return true;
		return false;
	}
	else return false;
}

void TCompCamera::debugInMenu() {
	ImGui::DragFloat("Distancia", &distance, 0.1f, -200.f, 200.f);
	ImGui::DragFloat("AP", &apertura, 0.1f, -2000.f, 2000.f);
	ImGui::DragFloat("OS", &xOffset, 0.1f, -2000.f, 2000.f);
	ImGui::DragFloat("Altura", &height, 0.1f, -20.f, 20.f);
	ImGui::DragFloat("Fov", &fov_deg, 0.1f, -1000.f, 1000.f);
	ImGui::DragFloat("Look_X", &X, 0.1f, -100.f, 100.f);
	ImGui::DragFloat("Look_Y", &Z, 0.1f, -100.f, 100.f);
	ImGui::DragFloat("Look_Z", &Y, 0.1f, -100.f, 100.f);
	//ImGui::DragFloat3("Front", &front.x, 0.1f, -100.f, 100.f);
	//ImGui::DragFloat("DIR", &dir, 0.1f, -100.f, 100.f);

}

void TCompCamera::load(const json& j, TEntityParseContext& ctx) {

	// ..
	player = (CEntity *)getEntityByName("The Player");

	fov_deg = j.value("fov", 75.f);
	z_near = j.value("z_near", 0.1f);
	z_far = j.value("z_far", 1000.f);
	distance = j.value("distance", 5.f);
	height = j.value("height", 5.f);
	radio = j.value("radio", 15.f);
	setPerspective(deg2rad(fov_deg), z_near, z_far);

	X = 0;
	Y = 0;
	Z = 0;
	
	apertura = -300.f;

	//front = VEC3(0, 0, 0);
	//dir = 0;

	pForwarding = true;

	xOffset = deg2rad(((2 * 3.14159 * radio) / 360) * apertura);
}

void TCompCamera::update(float dt) {
	xOffset = deg2rad(((2 * 3.14159 * radio) / 360) * apertura);
	TCompTransform* c = get<TCompTransform>();
	assert(c);
	pos = c->getPosition();

	TCompTransform* p = player->get<TCompTransform>();
	assert(p);
	VEC3 pPos = p->getPosition();

	bool playerForward = isForward(pPos, p->getFront());  //Vemos si el player se esta moviendo hacia delante o hacia atras

	float os;

	VEC3 v = p->getLeft();
	if (playerForward) {
		//if (pForwarding)
		os = xOffset;
		pForwarding = true;
	}
	else {
		// . . .
		os = -xOffset;
		pForwarding = false;
	}

	VEC3 center = VEC3(0 + X, pPos.y + height + Y, 0 + Z);

	float d = VEC3::Distance(center, pPos);
	float _d = d / d;
	float x = pPos.x - _d * (center.x - pPos.x);
	float z = pPos.z - _d * (center.z - pPos.z);

	pos.x = x;
	pos.y = pPos.y + height;
	pos.z = z;
	//c->setPosition(pos);

	float _distance = VEC3::Distance(center, pos);

	float y, p2, _y, _p2;
	c->getYawPitchRoll(&y, &p2);
	p->getYawPitchRoll(&_y, &_p2);

	c->setPosition(center);

	y = _y + os;

	c->setYawPitchRoll(y, p2);
	VEC3 newPos = c->getPosition() + (c->getFront() * _distance);
	newPos.y = pPos.y + height;
	c->setPosition(newPos);

	this->setPerspective(deg2rad(fov_deg), z_near, z_far);
	this->lookAt(newPos, center, VEC3::UnitY);
}

