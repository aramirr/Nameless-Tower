#include "mcv_platform.h"
#include "entity/entity_parser.h"
#include "comp_circular_controller.h"
#include "comp_transform.h"

DECL_OBJ_MANAGER("circular_controller", TCompCircularController);

void TCompCircularController::debugInMenu() {
	ImGui::DragFloat("Speed", &speed, 0.1f, 0.f, 20.f);
	ImGui::Text("Yaw %f", rad2deg(curr_yaw));
}


void TCompCircularController::load(const json& j, TEntityParseContext& ctx) {
	speed = j.value("speed", 1.0f);

	carga = true;
}

void TCompCircularController::update(float dt) {

	curr_yaw += speed * dt / 5000;

	TCompTransform *c_my_transform = get<TCompTransform>();

	if (carga) {

		u = c_my_transform->getFront();
		u.Normalize();

		carga = false;
	}

	//VEC3 pos = c_my_transform->getPosition();

	MAT44 posM = c_my_transform->asMatrix();

	//c_my_transform->setPosition(VEC3(0, 0, 0));
	/*posM *= MAT44(1, 0, 0, pos.x, 0, 1, 0, pos.y, 0, 0, 1, pos.z, 0, 0, 0, 1);
	c_my_transform->setPosition(posM.Translation());
	posM = c_my_transform->asMatrix();*/

	//posM *= MAT44(cos(curr_yaw), -sin(curr_yaw), 0, 0, sin(curr_yaw), cos(curr_yaw), 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	posM *= MAT44(cos(curr_yaw) + pow(u.x, 2) * (1 - cos(curr_yaw)), u.x * u.y * (1 - cos(curr_yaw)) - u.z * sin(curr_yaw), u.x * u.z * (1 - cos(curr_yaw)) + u.y * sin(curr_yaw), 0,
				  u.y * u.x * (1 - cos(curr_yaw)) + u.z * sin(curr_yaw), cos(curr_yaw) + pow(u.y, 2) * (1 - cos(curr_yaw)), u.y * u.z * (1 - cos(curr_yaw)) - u.x * sin(curr_yaw), 0,
				  u.z * u.x * (1 - cos(curr_yaw)) - u.y * sin(curr_yaw), u.z * u.y * (1 - cos(curr_yaw)) + u.x * sin(curr_yaw), cos(curr_yaw) + pow(u.z, 2) * (1 - cos(curr_yaw)), 0,
				  0, 0, 0, 1);
	//c_my_transform->setPosition(posM.Translation());
	//posM = c_my_transform->asMatrix();

	//posM *= MAT44(1, 0, 0, -pos.x, 0, 1, 0, -pos.y, 0, 0, 1, -pos.z, 0, 0, 0, 1);

	c_my_transform->setRotation(QUAT::CreateFromRotationMatrix(posM));

	//c_my_transform->setPosition(pos);

	//QUAT rot = c_my_transform->getRotation();
	//VEC3 front = c_my_transform->getFront();
	//VEC3 pos = c_my_transform->getPosition();

	//rot.w += speed * dt;

	//float y, p, r;
	//c_my_transform->getYawPitchRoll(&y, &p, &r);

	//c_my_transform->setPosition(VEC3(0, 0, 0));

	//c_my_transform->setRotation(QUAT(rot.x, rot.y, rot.z, rot.w * -1));

	//c_my_transform->setYawPitchRoll(y, curr_yaw, r);

	//c_my_transform->setRotation(QUAT(0, 0, rot.z, rot.w));

	//c_my_transform->setPosition(pos);

	//std::string str = std::to_string(pos.x) + " - " + std::to_string(pos.y) + " - " + std::to_string(pos.z);
	//std::string str = std::to_string(rot.w);

	//dbg("------------------------------------------------------------------------------\n");
	//dbg(str.c_str());
	//dbg("\n");

	assert(c_my_transform);
}

