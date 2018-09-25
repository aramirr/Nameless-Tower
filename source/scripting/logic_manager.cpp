#include "mcv_platform.h"
#include <lua/SLB/SLB.hpp>
#include "components/player/comp_player_controller.h"
#include "logic_manager.h"
#include "components/juan/comp_transform.h"
#include "components/juan/comp_render.h"
#include "components/camera/comp_camera_manager.h"
using namespace SLB;


LogicManager::LogicManager() {}


void LogicManager::printDbg(const char* msg) {
	dbg(msg);
}

// Hide - Unhide Meshes
void LogicManager::disappearEntity(const char* name) {
  EngineTower.disappearEntity(name);
}

void LogicManager::appearEntity(const char* name) {
  EngineTower.appearEntity(name);
}

void LogicManager::renderOnlyShadows(const char* name, bool left) {
  if (applyFunction(left))
      EngineTower.renderOnlyShadows(name);
}

void LogicManager::renderEverything(const char* name, bool left) {
    if (applyFunction(left))
        EngineTower.renderEverything(name);
}

// Cinematics
void LogicManager::activateCinematic(const char* name, bool left) {
    if (applyFunction(left))
        EngineTower.activateCinematic(name);
}

void LogicManager::deactivateCinematic(const char* name, bool left) {
    if (applyFunction(left))
        EngineTower.deactivateCinematic(name);
}

// Misc
void LogicManager::setAmbientAdjustment(float value) {
	EngineTower.setAmbientAdjustment(value);
};

void LogicManager::setExposureAdjustment(float value) {
    EngineTower.setExposureAdjustment(value);
};

void LogicManager::playLastCinematic() {
	CEntity* cam = (CEntity*)getEntityByName("camera_manager");

	TCompCameraManager* cm = cam->get<TCompCameraManager>();
	assert(cm);

	cm->activateCinematic("final");
}

void LogicManager::openDoor(const char* name) {
  EngineTower.openDoor(name);
}

void LogicManager::activateAnim(const char* name) {
  EngineTower.activateAnim(name);
}

// UI
void LogicManager::activateText(const char* name) {
	std::string s = name;
	EngineUI.activateWidget(s);
}

void LogicManager::disactivateText(const char* name) {
	std::string s = name;
	EngineUI.desactivateWidget(s);
}

// Lights
void LogicManager::setDirLightIntensity(const char* name, float value, bool left) {
    if (applyFunction(left))
        EngineTower.setDirLightIntensity(name, value);
}

void LogicManager::setPointLightIntensity(const char* name, float value, bool left) {
    if (applyFunction(left))
        EngineTower.setPointLightIntensity(name, value);
}

// Change Level
void LogicManager::changeGameState(const char* name) {
    
    CEntity* e = getEntityByName("The Player");
    TCompPlayerController* player = e->get<TCompPlayerController>();
    if (player->game_state == "level_1") {
        CEngine::get().getModules().changeGameState(name);
        player->game_state = "level_2";
    }        
}

bool LogicManager::applyFunction(bool left) {
    CEntity* e = getEntityByName("The Player");
    TCompPlayerController* player = e->get<TCompPlayerController>();
    return player->looking_left == left;
}