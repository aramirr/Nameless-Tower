#include "mcv_platform.h"
#include "entity/entity_parser.h"
#include "comp_circular_controller.h"
#include "components/juan/comp_transform.h"

DECL_OBJ_MANAGER("circular_controller", TCompCircularController);

void TCompCircularController::debugInMenu() {
	ImGui::DragFloat("Speed", &speed, 0.1f, 0.f, 20.f);
}


void TCompCircularController::load(const json& j, TEntityParseContext& ctx) {
	speed = j.value("speed", 1.0f) * 0.0001f;
	eje = *((j.value("eje", "X")).c_str());

	carga = true;
}

void TCompCircularController::update(float dt) {

	float DEG = rad2deg(speed);
	float COS = cos(DEG);
	float SIN = sin(DEG);

	TCompTransform *c_my_transform = get<TCompTransform>();

	if (carga) {    //CARGA INICIAL

		if (eje == 'X')u = c_my_transform->getFront();
		else if (eje == 'Y')u = c_my_transform->getUp();
		else if (eje == 'Z')u = c_my_transform->getLeft();
		u.Normalize();

		carga = false;
	}

	MAT44 posM = c_my_transform->asMatrix();

	posM *= MAT44(COS + pow(u.x, 2) * (1 - COS), u.x * u.y * (1 - COS) - u.z * SIN, u.x * u.z * (1 - COS) + u.y * SIN, 0,
		u.y * u.x * (1 - COS) + u.z * SIN, COS + pow(u.y, 2) * (1 - COS), u.y * u.z * (1 - COS) - u.x * SIN, 0,
		u.z * u.x * (1 - COS) - u.y * SIN, u.z * u.y * (1 - COS) + u.x * SIN, COS + pow(u.z, 2) * (1 - COS), 0,
		0, 0, 0, 1);

	c_my_transform->setRotation(QUAT::CreateFromRotationMatrix(posM));

	assert(c_my_transform);
}

