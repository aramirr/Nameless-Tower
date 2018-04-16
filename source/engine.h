#pragma once

#include "modules/module_manager.h"
#include "modules/module_render.h"
#include "modules/module_entities.h"
#include "modules/system/module_physics.h"
#include "modules/module_ia.h"
#include "modules/system/module_input.h"
#include "modules/system/module_cameras.h"
#include "modules/system/module_fsm.h"
#include "modules/system/module_gui.h"

class CEngine
{
public:
	CEngine();
  static CEngine& get();

  bool start();
  bool stop();
  void update(float delta);
  void render();

  CModuleManager& getModules() { return _modules; }
  CModuleRender& getRender() { return _module_render; }
  CModuleIA& getIA() { return _module_ia; }
  CModuleInput& getInput() { return _module_input; }
  CModulePhysics& getPhysics() { return _module_physics; }
  CModuleCameras& getCameras() { return _module_cameras; }
  CModuleFSM& getFSM() { return _module_fsm; }
  CModuleGUI& getGUI() { return _module_gui; }

  float getUnscaledDeltaTime() const { return current_unscaled_delta_time; }

private:
  CModuleManager  _modules;
  CModuleRender   _module_render;
  CModuleEntities _module_entities;
  CModulePhysics  _module_physics;
	CModuleIA       _module_ia;
	CModuleInput    _module_input;
  CModuleCameras  _module_cameras;
  CModuleFSM      _module_fsm;
  CModuleGUI      _module_gui;
  
  float           current_unscaled_delta_time = 0.f;
};

#define Engine CEngine::get()
#define EngineInput CEngine::get().getInput()
