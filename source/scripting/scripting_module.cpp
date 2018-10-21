#include "mcv_platform.h"
#include "scripting_module.h"
#include "logic_manager.h"
#include <lua/SLB/SLB.hpp>
using namespace SLB;
using namespace std;

ScriptingModule::ScriptingModule(const string& name): logic_manager(), script(&logic_manager)
{
	this->name = name;
	BootLuaSLB(&logic_manager);
	script.mcvSetManager(&logic_manager);
    script.doFile("data/scripts/scripts.lua");
}

void ScriptingModule::BootLuaSLB(SLB::Manager *m)
{
	SLB::Class< LogicManager >("LogicManager", m)
		.comment("This is our wrapper of LogicManager class")
		.constructor()
		.set("printdbg", &LogicManager::printDbg)
		.set("hide_mesh", &LogicManager::disappearEntity)
		.set("unhide_mesh", &LogicManager::appearEntity)
		.set("render_only_shadows", &LogicManager::renderOnlyShadows)
		.set("render_everything", &LogicManager::renderEverything)
		.set("activate_cinematic", &LogicManager::activateCinematic)
		.set("deactivate_cinematic", &LogicManager::deactivateCinematic)
		.set("activate_cinematic_bands", &LogicManager::activateBandCinematics)
		.set("deactivate_cinematic_bands", &LogicManager::deactivateBandCinematics)
		.set("set_ambient", &LogicManager::setAmbientAdjustment)
		.set("set_exposure", &LogicManager::setExposureAdjustment)
		.set("play_last_cinematic", &LogicManager::playLastCinematic)
		.set("set_temblor", &LogicManager::setTemblor)
		.set("activate_text", &LogicManager::activateText)
		.set("disactivate_text", &LogicManager::disactivateText)
		.set("set_dir_light_intensity", &LogicManager::setDirLightIntensity)
		.set("set_point_light_intensity", &LogicManager::setPointLightIntensity)
		.set("open_door", &LogicManager::openDoor)
    .set("close_door", &LogicManager::closeDoor)
    .set("activate_anim", &LogicManager::activateAnim)
    .set("change_level", &LogicManager::changeGameState)
    .set("play_ambient_sound", &LogicManager::playAmbientSound)
    .set("play_interior_sound", &LogicManager::playInteriorSound)
    .set("play_sound", &LogicManager::playSound)
		.set("set_naja_int", &LogicManager::setNajaInterior)
		.set("set_naja_ext", &LogicManager::setnajaExterior)
		.set("set_runner_int", &LogicManager::setRunnerInterior)
		.set("set_runner_ext", &LogicManager::setRunnerExterior)
		.set("start_emiter", &LogicManager::startEmiter)
		.set("stop_emiter", &LogicManager::stopEmiter)
		.set("pause_player", &LogicManager::pausePlayer)
		.set("play_animation_player", &LogicManager::setAnim)
		.set("play_animation", &LogicManager::setAnimCycle)
		.set("regain_control", &LogicManager::regainControl)
		.set("set_cinematic_bars", &LogicManager::setCinematicBars)
		.set("kill_entity", &LogicManager::killEntity)
		.set("insert_grass", &LogicManager::addGrass)
		.set("delete_grass", &LogicManager::deleteGrass)
		.set("update_grass_render", &LogicManager::updateGrassRender)
    .set("stop_sound", &LogicManager::stopSound)
    .set("activate_torch", &LogicManager::activateTorch)
    .set("scare_player", &LogicManager::scarePlayer)
		.set("play_positional_sound", &LogicManager::playPositionalSound)
		.set("play_ambient_night", &LogicManager::playAmbientNight)
		.set("play_curve", &LogicManager::playCurve)
		.set("set_ypr", &LogicManager::set_ypr)
		.set("activate_runner", &LogicManager::activateRunner)
		.set("play_delayed_sound", &LogicManager::playDelayedSound)
		;
}

void ScriptingModule::ExecEvent(ScriptEvents e, vector<string> params) {
	/*switch (e) {
    case ScriptEvents::trigger_enter: {
      fTriggerEnter(params);
    }
    case ScriptEvents::windstrike_activate: {
      fWindstrikeActivate(params);
    }
	}*/
  if (e == ScriptEvents::trigger_exit) {
    fTriggerExit(params);
  }
  else if (e == ScriptEvents::windstrike_activate) {
    fWindstrikeActivate(params);
  }
		
}


void ScriptingModule::fTriggerExit(vector<string> params) {
    if (params.size() > 0) {
        string name = params[0];
        string func_name = "OnTriggerExit" + name;
        auto p = EngineScripting.script.exists(func_name);
				if (p && !(name == "TSCinematicaMonolito" && done_cinematic_monolito)) {
					if (name == "TSCinematicaMonolito")
						done_cinematic_monolito = true;
					EngineScripting.script.doString(func_name + "()");
				}
    }
}

void ScriptingModule::fWindstrikeActivate(vector<string> params) {
  if (params.size() > 0) {
    string name = params[0];
    string func_name = "OnWindstrikeActivate" + name;
    auto p = EngineScripting.script.exists(func_name);
    if (p)
      EngineScripting.script.doString(func_name + "()");
  }
}
