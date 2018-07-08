#include "mcv_platform.h"
#include <lua/SLB/SLB.hpp>

#include "logic_manager.h"
#include "components/juan/comp_transform.h"
#include "components/juan/comp_render.h"
#include "components/camera/comp_camera_manager.h"
using namespace SLB;


LogicManager::LogicManager() {}


void LogicManager::printDbg(const char* msg) {
	dbg(msg);
}

void LogicManager::disappearEntity(const char* name) {
	EngineTower.disappearEntity(name);
}

void LogicManager::appearEntity(const char* name) {
	EngineTower.appearEntity(name);
}

void LogicManager::activateCamera(const char* name) {
	dbg("JOHAN");
}

void LogicManager::disactivateCamera(const char* name) {
	dbg("JOHAN");
}

void LogicManager::setAmbientAdjustment(float value) {
	EngineTower.setAmbientAdjustment(value);
};

void LogicManager::playLastCinematic() {
	CEntity* cam = (CEntity*)getEntityByName("camera_manager");

	TCompCameraManager* cm = cam->get<TCompCameraManager>();
	assert(cm);

	cm->activateCinematic("final");
}

void LogicManager::activateText(const char* name) {
	std::string s = name;
	EngineUI.activateWidget(s);
}

void LogicManager::disactivateText(const char* name) {
	std::string s = name;
	EngineUI.desactivateWidget(s);
}

void LogicManager::setDirLightIntensity(const char* name, float value) {
  EngineTower.setDirLightIntensity(name, value);
}

void LogicManager::setPointLightIntensity(const char* name, float value) {
  EngineTower.setPointLightIntensity(name, value);
}

void LogicManager::openDoor(const char* name) {
  EngineTower.openDoor(name);
}

LogicPlayer::LogicPlayer() {}

void LogicPlayer::setPlayerPosition(float x, float y, float z) {
	CEntity* player = (CEntity*)getEntityByName("The Player");
	TCompCollider *player_collider = player->get<TCompCollider>();
	PxRigidActor* rigidActor = ((PxRigidActor*)player_collider->actor);
	PxTransform tr = rigidActor->getGlobalPose();
	tr.p = PxVec3(x, y, z);
	rigidActor->setGlobalPose(tr);
}