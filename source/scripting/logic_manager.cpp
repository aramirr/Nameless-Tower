#include "mcv_platform.h"
#include <lua/SLB/SLB.hpp>
#include "components/player/comp_player_controller.h"
#include "components/sound/comp_sound.h"
#include "logic_manager.h"
#include "components/juan/comp_transform.h"
#include "components/juan/comp_render.h"
#include "components/comp_particles.h"
#include "components/camera/comp_camera_manager.h"
#include "components/controllers/comp_curve.h"
#include "skeleton/comp_skeleton.h"
#include "render\render_objects.h"
#include "ctes.h"
#include "render/cte_buffer.h"
#include "skeleton/comp_skeleton.h"
#include "entity/entity_parser.h"
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

void LogicManager::activateBandCinematics(bool left) {
	if (applyFunction(left))
		EngineTower.setBandsCinematics(true);
}

void LogicManager::deactivateBandCinematics(bool left) {
	if (applyFunction(left))
		EngineTower.setBandsCinematics(false);
}

// Misc
void LogicManager::setAmbientAdjustment(float value) {
	EngineTower.setAmbientAdjustment(value);
};

void LogicManager::setExposureAdjustment(float value, bool left) {
	if (applyFunction(left))
    EngineTower.setExposure(value);
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

void LogicManager::closeDoor(const char* name) {
    EngineTower.closeDoor(name);
}

void LogicManager::activateAnim(const char* name, float wait_time) {
  EngineTower.activateAnim(name, wait_time);
}

void LogicManager::setTemblor(bool temblor, bool left)
{
	if (applyFunction(left)) {
		CEntity* cam = (CEntity*)getEntityByName("camera_manager");

		TCompCameraManager* cm = cam->get<TCompCameraManager>();
		assert(cm);

		if(temblor)cm->activarTemblor();
		else cm->desactivarTemblor();
	}
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

void LogicManager::activateCredits()
{
	cb_gui.creditos = true;
}

void LogicManager::desactivateCinematics()
{
	cb_gui.cinematica = false;
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

void LogicManager::setNajaInterior(bool left) {
	if (applyFunction(left)) {
		cb_globals.global_naja_interior = 1;

		EngineTower.setExposure(0.045f);
		//cb_globals.global_exposure_adjustment = 0.045f;
		cb_globals.global_ambient_adjustment = 1.f;

		cb_globals.global_hdr_enabled = 1.f;
		cb_globals.global_gamma_correction_enabled = 1.f;
		cb_globals.global_tone_mapping_mode = 0.760f;

		cb_globals.global_hue_adjustment = 1.f;
		cb_globals.global_sat_adjustment = 1.4f;
		EngineTower.setLight(1.630f);
		cb_globals.global_vignetting_adjustment = 0.250f;
		cb_globals.global_fogDensity_adjustment = 0.f;

		cb_globals.global_contrast_adjustment = 0.265f;


		cb_globals.global_fog_percentage_horizontal = 0.150f;
		cb_globals.global_fog_percentage_vertical = 0.150f;
	}
		
}

void LogicManager::setnajaExterior(bool left) {
	if (applyFunction(left)) {
		cb_globals.global_naja_interior = 0;

		EngineTower.setExposure(0.340f);
		//cb_globals.global_exposure_adjustment = 0.045f;
		cb_globals.global_ambient_adjustment = 0.700f;

		cb_globals.global_hdr_enabled = 1.f;
		cb_globals.global_gamma_correction_enabled = 1.f;
		cb_globals.global_tone_mapping_mode = 1.f;

		cb_globals.global_hue_adjustment = 1.f;
		cb_globals.global_sat_adjustment = 1.1f;
		EngineTower.setLight(0.110f);
		cb_globals.global_vignetting_adjustment = 0.300f;
		cb_globals.global_fogDensity_adjustment = 0.f;

		cb_globals.global_contrast_adjustment = 0.220f;


		cb_globals.global_fog_percentage_horizontal = 0.130f;
		cb_globals.global_fog_percentage_vertical = 0.120f;
	}
		
}

void LogicManager::setRunnerInterior(bool left)
{
	if (applyFunction(left))
		cb_globals.global_runner_interior = 1;
}

void LogicManager::setRunnerExterior(bool left)
{
	if (applyFunction(left))
		cb_globals.global_runner_interior = 0;
}

void LogicManager::startEmiter(const char* name, bool left) {
	if (applyFunction(left)) {
		CEntity* particles_emiter = (CEntity*)getEntityByName(name);
		TCompParticles* c_particles = particles_emiter->get<TCompParticles>();
		c_particles->start_emiter();
	}
}

void LogicManager::setVignettingAdjustment(float value, bool left)
{
	if (applyFunction(left))
		EngineTower.setVignettingAdjustment(value);
}
void LogicManager::setFadeOutAdjustment(float value, bool left)
{
	if (applyFunction(left))
		EngineTower.setFadeOutAdjustment(value);
}

void LogicManager::stopEmiter(const char* name, bool left) {
	if (applyFunction(left)) {
		CEntity* particles_emiter = (CEntity*)getEntityByName(name);
		TCompParticles* c_particles = particles_emiter->get<TCompParticles>();
		c_particles->stop_emiter();
	}
}

void LogicManager::addGrass(int id, bool left) {
	if (applyFunction(left))
		EngineBillboards.add_grass_id(id);
}

void LogicManager::deleteGrass(int id, bool left) {
	if (applyFunction(left))
		EngineBillboards.delete_grass_id(id);
}

void LogicManager::updateGrassRender() {
	EngineBillboards.set_grass_instances_2_render();
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

void LogicManager::playAmbientSound(bool left) {
    if (applyFunction(left)) {
        EngineSound.playAmbient();
    }
}

void LogicManager::playInteriorSound(bool left) {
    if (applyFunction(left)) {
        EngineSound.playInterior();
    }
}

void LogicManager::playSound(bool left, std::string name) {
    if (applyFunction(left)) {
        EngineSound.emitEvent(name);
    }
}

void LogicManager::playDelayedSound(float time, std::string name) {
	EngineSound.emitDelayedEvent(time, name);
}

void LogicManager::playPositionalSound(std::string name, std::string entityName) {    
    EngineSound.emitPositionalEvent(name, entityName);
}

void LogicManager::playAmbientNight() {
	EngineSound.emitEvent("ambient_night");
}


void LogicManager::stopSound(bool left, std::string name) {
    if (applyFunction(left)) {
        EngineSound.stopEvent(name);
    }
}

// LUA Cinematic
void LogicManager::setCinematicBars() {
	//to be implemented
	//EngineTower.toggle_cinematic_bars();
}

void LogicManager::pausePlayer() {
	CEntity* player = getEntityByName("The Player");

	TMsgSetFSMVariable pauseMsg;
	pauseMsg.variant.setName("pause");
	pauseMsg.variant.setBool(true);
	player->sendMsg(pauseMsg);

	//Pinchar la camara que sea sobre railes aqui

}

void LogicManager::setAnim(int anim_id) {
	CEntity* e = getEntityByName("The Player");
	TCompPlayerController * controller = e->get<TCompPlayerController>();
	if (controller != nullptr) {
		controller->remove_animation(controller->EAnimations::NajaJumpLoop);
	} 
	TCompSkeleton* skel = e->get<TCompSkeleton>();
	skel->playAnimation(anim_id, true, 0, 0.8, true);
}

void LogicManager::setAnimCycle(std::string name, int anim_id) {
	CEntity* e = getEntityByName(name);
	TCompSkeleton * skeleton = e->get<TCompSkeleton>();
	skeleton->playAnimation(anim_id, false, 0.f, 0.f, true);
}


void LogicManager::regainControl(float time_to_wait) {
	// La function debe devolver el control al personaje, sacar las barras y 
	EngineTower.wait_seconds(time_to_wait);
}

void LogicManager::killEntity(std::string name) {
	CEntity* entity = getEntityByName(name);
	if (entity) {
		CHandle(entity).destroy();
	}
}

void LogicManager::activateTorch(std::string name) {
    CEntity* e = getEntityByName(name);
    TMsgActivateTorch msg;
    e->sendMsg(msg);
}

void LogicManager::scarePlayer() {
    CEntity* e = getEntityByName("The Player");
    TMsgSetFSMVariable scareMsg;
    scareMsg.variant.setName("scared");
    scareMsg.variant.setBool(true);
    e->sendMsg(scareMsg);
}

void LogicManager::playCurve(std::string name) {
    CEntity* entity = getEntityByName(name);
    TCompCurve* t = entity->get<TCompCurve>();
    t->activate();
}

void LogicManager::set_ypr(std::string name, float y, float p, float r) {
	CEntity* entity = getEntityByName(name);
	TCompTransform* t = entity->get<TCompTransform>();
	t->setYawPitchRoll(y, p, r);
}

void LogicManager::activateRunner() {
	EngineTower.activateRunner();
}

void LogicManager::unloadScene(std::string name) {
	TEntityParseContext ctx;
	deleteScene(name, ctx);
}

void LogicManager::loadScene(std::string name) {
	TEntityParseContext ctx;
	parseScene(name, ctx);
}
void LogicManager::setVolumen(float volumen, bool left)
{
	if (applyFunction(left)) {
		EngineSound.setVolumen(volumen);
	}
}

void LogicManager::fundidoNegroFinal() {
	EngineTower.endGame();
}
