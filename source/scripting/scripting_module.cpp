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
		.set("activate_camera", &LogicManager::activateCamera)
		.set("disactivate_camera", &LogicManager::disactivateCamera)
    .set("activate_cinematic", &LogicManager::activateCinematic)
    .set("deactivate_cinematic", &LogicManager::deactivateCinematic)
		.set("set_ambient", &LogicManager::setAmbientAdjustment)
		.set("play_last_cinematic", &LogicManager::playLastCinematic)
		.set("activate_text", &LogicManager::activateText)
    .set("disactivate_text", &LogicManager::disactivateText)
    .set("set_dir_light_intensity", &LogicManager::setDirLightIntensity)
    .set("set_point_light_intensity", &LogicManager::setPointLightIntensity)
		.set("open_door", &LogicManager::openDoor)
		.set("activate_anim", &LogicManager::activateAnim)
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
  if (e == ScriptEvents::trigger_enter) {
    fTriggerEnter(params);
  }
  else if (e == ScriptEvents::windstrike_activate) {
    fWindstrikeActivate(params);
  }
		
}


void ScriptingModule::fTriggerEnter(vector<string> params) {
  if (params.size() > 0) {
    string name = params[0];
    string func_name = "OnTriggerEnter" + name;
    auto p = EngineScripting.script.exists(func_name);
    if (p)
      EngineScripting.script.doString(func_name + "()");
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
