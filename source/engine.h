#pragma once

#include "modules/module_manager.h"
#include "modules/module_render.h"
#include "modules/module_entities.h"
#include "modules/system/module_physics.h"
#include "modules/module_ia.h"
#include "modules/system/module_input.h"
#include "modules/module_timer.h"
#include "modules/system/module_cameras.h"
#include "modules/system/module_cinematics.h"

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
	CModulePhysics& getPhysics() { return _module_physics; }
	CModuleCameras& getCameras() { return _module_cameras; }
	CModuleCinematics& getCinematics() { return _module_cinematics; }

private:
	CModuleManager  _modules;
	CModuleRender   _module_render;
	CModuleEntities _module_entities;
	CModulePhysics  _module_physics;
	CModuleIA       _module_ia;
	CModuleInput    _module_input;
	CModuleTimer    _module_timer;
	CModuleCameras  _module_cameras;
	CModuleCinematics  _module_cinematics;
};

#define Engine CEngine::get()
#define EngineInput CEngine::get().getInput()
#define EngineTimer CEngine::get().getTimer()
#define EngineCinematics CEngine::get().getCinematics()
