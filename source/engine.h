#pragma once

#include "modules/module_manager.h"
#include "modules/module_render.h"
#include "modules/module_entities.h"
#include "modules/system/module_physics.h"
#include "modules/module_ia.h"
#include "modules/system/module_input.h"
#include "modules/module_timer.h"
#include "modules/module_tower.h"
#include "modules/system/module_cameras.h"
#include "modules/system/module_fsm.h"


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
	CModuleTimer& getTimer() { return _module_timer; }
	CModuleTower& getTower() { return _module_tower; }
	CModulePhysics& getPhysics() { return _module_physics; }
	CModuleCameras& getCameras() { return _module_cameras; }
	CModuleEntities& getEntities() { return _module_entities; }
	CModuleFSM& getFSM() { return _module_fsm; }

	float getUnscaledDeltaTime() const { return current_unscaled_delta_time; }

  float getUnscaledDeltaTime() const { return current_unscaled_delta_time; }

private:
	CModuleManager  _modules;
	CModuleRender   _module_render;
	CModuleEntities _module_entities;
	CModulePhysics  _module_physics;
	CModuleIA       _module_ia;
	CModuleInput    _module_input;
	CModuleTimer    _module_timer;
	CModuleTower    _module_tower;
	CModuleCameras  _module_cameras;
	CModuleFSM      _module_fsm;

	float           current_unscaled_delta_time = 0.f;
};

#define Engine CEngine::get()
#define EngineInput CEngine::get().getInput()
#define EngineTimer CEngine::get().getTimer()
#define EngineTower CEngine::get().getTower()
