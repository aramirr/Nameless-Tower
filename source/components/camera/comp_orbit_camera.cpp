#include "mcv_platform.h"
#include "comp_orbit_camera.h"
#include "components/juan/comp_transform.h"
#include "components/player/comp_player_controller.h"
#include "render/render_objects.h"
#include "render/render_utils.h"
#include "render/render_manager.h"

DECL_OBJ_MANAGER("orbitCamera", TCompOrbitCamera);

bool TCompOrbitCamera::isForward() {
	TCompPlayerController* pc = player->get<TCompPlayerController>();
	return !pc->isForward();
}

bool TCompOrbitCamera::isGrounded() {
	TCompPlayerController* pc = player->get<TCompPlayerController>();
	return pc->isGrounded();

}

void TCompOrbitCamera::activateCamera(const TMsgActiveCamera & msg)
{
	active = true;
}

void TCompOrbitCamera::deactivateCamera(const TMsgDeactiveCamera & msg)
{
	active = false;
}

void TCompOrbitCamera::blockCamera(const TMsgBlockCamera & msg)
{
	block = true;
}

void TCompOrbitCamera::registerMsgs() {
	DECL_MSG(TCompOrbitCamera, TMsgActiveCamera, activateCamera);
	DECL_MSG(TCompOrbitCamera, TMsgDeactiveCamera, deactivateCamera);
	DECL_MSG(TCompOrbitCamera, TMsgBlockCamera, blockCamera);
}

void TCompOrbitCamera::debugInMenu() {
	ImGui::Text("Delante: %s", isForward() ? "Si" : "No");
	ImGui::Text("Suelo: %s", isGrounded() ? "Si" : "No");
	ImGui::Text("Active: %s", active ? "Si" : "No");
	ImGui::DragFloat("Distancia", &distance, 0.1f, -200.f, 200.f);
	ImGui::DragFloat("AP", &apertura, 0.1f, -2000.f, 2000.f);
	ImGui::DragFloat("Altura", &height, 0.1f, -20.f, 20.f);
	ImGui::DragFloat("Fov", &fov_deg, 0.1f, -1000.f, 1000.f);
	ImGui::DragFloat("Look_X", &X, 0.1f, -100.f, 100.f);
	ImGui::DragFloat("Look_Y", &Y, 0.1f, -100.f, 100.f);
	ImGui::DragFloat("Look_Z", &Z, 0.1f, -100.f, 100.f);
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
	apertura = j.value("apertura", -278.f);
	blockY = j.value("blockY", false);
	blockX = j.value("blockX", 0.f);
	blockZ = j.value("blockZ", 0.f);

	X = j.value("x", 0);
	Y = j.value("y", 0);
	Z = j.value("z", 0);

	TCompTransform* p = player->get<TCompTransform>();
	assert(p);
	VEC3 pPos = p->getPosition();

	playerY = pPos.y;
	currentPlayerY = pPos.y;

	offset = deg2rad(((2 * 3.14159f * radio) / 360) * apertura);

	speedCaida = 0.f;
	pesoOldPosition = 0.75f;

	carga = true;
	active = true;
	block = false;

	camera = get<TCompCamera>();
}

void TCompOrbitCamera::update(float dt) {
	offset = deg2rad(((2 * 3.14159f * radio) / 360) * apertura);

	TCompTransform* c = get<TCompTransform>();
	assert(c);
	VEC3 pos = c->getPosition();

	VEC3 newPos;

	bool b_caida = false;

	TCompTransform* p = player->get<TCompTransform>();
	assert(p);
	VEC3 pPos = p->getPosition();

	float dY = abs(oldPos.y - pPos.y);

	VEC3 center = VEC3(0 + X, currentPlayerY + height + Y, 0 + Z);

	if (cb_globals.global_bajada == 1.f)center.y = currentPlayerY + Y;

	if (!block) {
		if (currentPlayerY < pPos.y - 0.1f) {
			//b_caida = false;
			if (dY > height + height + 1.5f) {
				currentPlayerY += 1.5f;
				speedCaida = 40.f;
			}
			else if (dY > height + (height / 2) + 1.5f) {
				currentPlayerY += 1.5f;
				speedCaida = 40.f;
			}
			else if (dY > height + (height / 3) + 1.5f) {
				currentPlayerY += 1.5f;
				speedCaida = 40.f;
			}
			else if (dY > height + (height / 4) + 1.5f) {
				currentPlayerY += 1.5f;
				speedCaida = 40.f;
			}
			else {
				//Raycast looking for walls
				VEC3 player_position = p->getPosition();
				VEC3 player_Up = p->getUp();

				//DOWN
				VEC3 posef = player_position - player_Up * 0.3f;
				PxVec3 originf = PxVec3(posef.x, posef.y + 0.5f, posef.z);
				PxVec3 unitDirf = PxVec3(-player_Up.x, -player_Up.y, -player_Up.z);

				PxReal maxDistance = 4.0f;
				PxRaycastBuffer hit;

				bool stairs = false;

				if (EnginePhysics.gScene->raycast(originf, unitDirf, maxDistance, hit)) {
					PxFilterData filter_data = hit.block.shape->getSimulationFilterData();
					if (filter_data.word0 == CModulePhysics::FilterGroup::Stairs) {
						stairs = true;
					}
				}

				if (stairs) currentPlayerY += 0.05f;
				else currentPlayerY = pPos.y;
				speedCaida = 0.f;
			}
		}
		else if (currentPlayerY > pPos.y + 0.1f) {
			if (dY > height + height + 1.5f) speedCaida = 40.f;
			else if (dY > height + (height / 2) + 1.5f) {
				currentPlayerY = pPos.y;
				speedCaida = 30.f;
			}
			else if (dY > height + (height / 3) + 1.5f) {
				currentPlayerY = pPos.y;
				speedCaida = 20.f;
			}
			else if (dY > height + (height / 4) + 1.5f) {
				currentPlayerY = pPos.y;
				speedCaida = 10.f;
			}
			else if (dY > height + (height / 5) + 1.5f) {
				currentPlayerY = pPos.y;
				speedCaida = 5.f;
			}
			else {
				//Raycast looking for walls
				VEC3 player_position = p->getPosition();
				VEC3 player_Up = p->getUp();

				//DOWN
				VEC3 posef = player_position - player_Up * 0.3f;
				PxVec3 originf = PxVec3(posef.x, posef.y + 0.5f, posef.z);
				PxVec3 unitDirf = PxVec3(-player_Up.x, -player_Up.y, -player_Up.z);

				PxReal maxDistance = 4.0f;
				PxRaycastBuffer hit;

				bool stairs = false;

				if (EnginePhysics.gScene->raycast(originf, unitDirf, maxDistance, hit)) {
					PxFilterData filter_data = hit.block.shape->getSimulationFilterData();
					if (filter_data.word0 == CModulePhysics::FilterGroup::Stairs) {
						stairs = true;
					}
				}

				if (stairs) currentPlayerY -= 0.04f;
				else currentPlayerY = pPos.y;
				speedCaida = 0.f;
			}
			b_caida = true;
		}

		center = VEC3(0 + X, currentPlayerY + height + Y, 0 + Z);
		if (cb_globals.global_bajada == 1.f)center.y = currentPlayerY + Y;
		if (active) {
			float d = VEC3::Distance(center, pPos);
			float _d = (d - distance) / d;
			float x = pPos.x - _d * (center.x - pPos.x);
			float z = pPos.z - _d * (center.z - pPos.z);

			pos.x = x;
			pos.y = currentPlayerY + height;
			if (cb_globals.global_bajada == 1.f)pos.y = currentPlayerY;
			pos.z = z;
			float _distance = VEC3::Distance(center, pos);

			if (blockY) {
				pos.x = blockX;
				pos.y = currentPlayerY + height;
				if (cb_globals.global_bajada == 1.f)pos.y = currentPlayerY;
				pos.z = blockZ;
				//_distance = VEC3::Distance(center, pos);
				c->setPosition(center);
				c->lookAt(pos,center);
				c->setPosition(pos);
				newPos = c->getPosition() - (c->getFront() * distance);
			}
			else {
				if (isForward())offset *= -1;

				float y, p2, _y, _p2;
				c->getYawPitchRoll(&y, &p2);
				p->getYawPitchRoll(&_y, &_p2);

				c->setPosition(center);

				y = _y + offset;
				c->setYawPitchRoll(y, p2);
				newPos = c->getPosition() - (c->getFront() * _distance);
			}

			
			newPos.y = currentPlayerY + height;
			if (cb_globals.global_bajada == 1.f)newPos.y = currentPlayerY;
		}
		else {
			newPos = oldPos;
			newPos.y = currentPlayerY + height;
			if (cb_globals.global_bajada == 1.f)newPos.y = currentPlayerY;
		}
	}
	else {
		newPos = oldPos;
		newPos.y = currentPlayerY + height;
		if (cb_globals.global_bajada == 1.f)newPos.y = currentPlayerY;
	}
	if (!carga) {
		actualPos = VEC3::Lerp(oldPos, newPos, dt *(10 + speedCaida));
		if (b_caida == true) {
			pesoOldPosition = 0.1f;
		}
		else {
			pesoOldPosition = 0.75f;
		}
		float pesoNewPosition = (1 - pesoOldPosition);
		//newPos = pesoOldPosition * oldPos + (1 - pesoOldPosition) * actualPos;
		newPos = VEC3(0.75f * oldPos.x, pesoOldPosition * oldPos.y, 0.75f * oldPos.z) + VEC3(0.25f * actualPos.x, pesoNewPosition * actualPos.y, 0.25f * actualPos.z);
	}
	else carga = false;

	oldPos = newPos;

	c->setPosition(newPos);
	c->lookAt(newPos, center);
}

void TCompOrbitCamera::setPosition(VEC3 position)
{
	//dbg("PRUEBA");
	active = false;
	oldPos = position;
	TCompTransform* c = get<TCompTransform>();
	assert(c);
	VEC3 center = VEC3(0 + X, currentPlayerY + height + Y, 0 + Z);
	if (cb_globals.global_bajada == 1.f)center.y = currentPlayerY + Y;
	c->setPosition(position);
	c->lookAt(position, center);
	carga = true;
	active = true;
	block = false;
}
