#include "mcv_platform.h"
#include "comp_camera.h"
#include "comp_transform.h"

DECL_OBJ_MANAGER("camera", TCompCamera);

void TCompCamera::debugInMenu() {
	ImGui::DragFloat("Distancia", &dist, 0.1f, -20.f, 20.f);
	ImGui::DragFloat("Altura", &h, 0.1f, -20.f, 20.f);
}

void TCompCamera::load(const json& j, TEntityParseContext& ctx) {

	// ..
	player = (CEntity *)getEntityByName("The Player");

	float fov_deg = j.value("fov", 75.f);
	float z_near = j.value("z_near", 0.1f);
	float z_far = j.value("z_far", 1000.f);
	setPerspective(deg2rad(fov_deg), z_near, z_far);
}

void TCompCamera::update(float dt) {
	TCompTransform* c = get<TCompTransform>();
	assert(c);
	pos = c->getPosition();

	TCompTransform* p = player->get<TCompTransform>();
	assert(p);
	VEC3 pPos = p->getPosition();

	VEC3 centre = VEC3(0, pPos.y + h, 0);
	
	float d = sqrt(pow(centre.x - pPos.x, 2) + pow(centre.z - pPos.z, 2));
	float _d = (dist + d) / d;
	float x = pPos.x - _d * (centre.x - pPos.x);
	float z = pPos.z - _d * (centre.z - pPos.z);

	pos.x = x;
	pos.y = pPos.y + h;
	pos.z = z;
	c->setPosition(pos);

	this->lookAt(pos, centre, VEC3::UnitY);
}

