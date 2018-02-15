#include "mcv_platform.h"
#include "comp_camera.h"
#include "comp_transform.h"

DECL_OBJ_MANAGER("camera", TCompCamera);

void TCompCamera::debugInMenu() {
	ImGui::DragFloat("Distancia", &dist, 0.1f, -20.f, 20.f);
	ImGui::DragFloat("Altura", &h, 0.1f, -20.f, 20.f);
	ImGui::DragFloat("Fov", &fov_deg, 0.1f, -1000.f, 1000.f);
	ImGui::DragFloat("Look_X", &X, 0.1f, -100.f, 100.f);
	ImGui::DragFloat("Look_Y", &Z, 0.1f, -100.f, 100.f);
	ImGui::DragFloat("Look_Z", &Y, 0.1f, -100.f, 100.f);

}

void TCompCamera::load(const json& j, TEntityParseContext& ctx) {

	// ..
	player = (CEntity *)getEntityByName("The Player");

	fov_deg = j.value("fov", 75.f);
	z_near = j.value("z_near", 0.1f);
	z_far = j.value("z_far", 1000.f);
	setPerspective(deg2rad(fov_deg), z_near, z_far);

	X = 0;
	Y = 0;
	Z = 0;
}

void TCompCamera::update(float dt) {
	TCompTransform* c = get<TCompTransform>();
	assert(c);
	pos = c->getPosition();

	TCompTransform* p = player->get<TCompTransform>();
	assert(p);
	VEC3 pPos = p->getPosition();

	VEC3 centre = VEC3(0 + X, pPos.y + h + Y, 0 + Z);
	
	float d = VEC3::Distance(centre, pPos);
	d = (dist + d) / d;
	float x = pPos.x - d * (centre.x - pPos.x);
	float z = pPos.z - d * (centre.z - pPos.z);

	pos.x = x;
	pos.y = pPos.y + h;
	pos.z = z;
	c->setPosition(pos);

	this->setPerspective(deg2rad(fov_deg), z_near, z_far);
	this->lookAt(pos, centre, VEC3::UnitY);
}

