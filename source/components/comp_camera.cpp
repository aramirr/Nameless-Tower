#include "mcv_platform.h"
#include "comp_camera.h"
#include "comp_transform.h"

DECL_OBJ_MANAGER("camera", TCompCamera);

void TCompCamera::debugInMenu() {

	float fov_deg = rad2deg(getFov());
	float new_znear = getZNear();
	float new_zfar = getZFar();
	bool changed = ImGui::DragFloat("Fov", &fov_deg, 0.1f, 30.f, 175.f);
	changed |= ImGui::DragFloat("Z Near", &new_znear, 0.001f, 0.01f, 1.0f);
	changed |= ImGui::DragFloat("Z Far", &new_zfar, 1.0f, 1.0f, 3000.0f);
	if (changed)
		setPerspective(deg2rad(fov_deg), new_znear, new_zfar);
}

void TCompCamera::load(const json& j, TEntityParseContext& ctx) {

	// ..
	player = (CEntity *)getEntityByName("The Player");

	float fov_deg = j.value("fov", rad2deg(getFov()));
	float z_near = j.value("z_near", getZNear());
	float z_far = j.value("z_far", getZFar());

	height = j.value("height", 5.f);

	setPerspective(deg2rad(fov_deg), z_near, z_far);

}

void TCompCamera::update(float dt) {

	TCompTransform* c = get<TCompTransform>();
	assert(c);
	pos = c->getPosition();

	TCompTransform* p = player->get<TCompTransform>();
	assert(p);
	VEC3 pPos = p->getPosition();

	VEC3 center = VEC3(0, pPos.y + height, 0);

	float d = VEC3::Distance(center, pPos) * 10;
	float _d = d / d;
	float x = pPos.x - _d * (center.x - pPos.x);
	float z = pPos.z - _d * (center.z - pPos.z);

	pos.x = x;
	pos.y = pPos.y + height;
	pos.z = z;

	//c->setPosition(pos);

	//this->setPerspective(deg2rad(fov_deg), z_near, z_far);
	//this->lookAt(pos, center, c->getUp());
	this->lookAt(c->getPosition(), c->getPosition() + c->getFront(), c->getUp());

}

