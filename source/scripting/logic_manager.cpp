#include "mcv_platform.h"
#include <lua/SLB/SLB.hpp>

#include "logic_manager.h"
#include "components/juan/comp_transform.h"
#include "components/juan/comp_render.h"
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


LogicPlayer::LogicPlayer() {}

void LogicPlayer::setPlayerPosition(float x, float y, float z) {
	CEntity* player = (CEntity*)getEntityByName("The Player");
	TCompCollider *player_collider = player->get<TCompCollider>();
	PxRigidActor* rigidActor = ((PxRigidActor*)player_collider->actor);
	PxTransform tr = rigidActor->getGlobalPose();
	tr.p = PxVec3(x, y, z);
	rigidActor->setGlobalPose(tr);
}