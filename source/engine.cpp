#include "mcv_platform.h"
#include "engine.h"

#include "modules/system/module_fsm.h"
#include "modules/game/module_splash.h"
#include "modules/game/module_main_menu.h"
#include "modules/game/module_gameover.h"
#include "modules/game/module_level_1.h"
#include "modules/game/module_level_2.h"
#include "modules/test/module_test_input.h"
#include "modules/test/module_test_cameras.h"
#include "modules/test/module_test_instancing.h"

//--------------------------------------------------------------------------------------
CEngine& CEngine::get() {
	static CEngine engine;
	return engine;
}

CEngine::CEngine()

	: _module_render("render")
	, _module_entities("entities")
	, _module_physics("physics")
	, _module_ia("ia")
	, _module_input("input")
	, _module_timer("timer")
	, _module_tower("tower")
	, _module_cameras("cameras")
	, _module_fsm("fsm")
	, _module_gui("gui")
	, _module_sound("sound")
	, _module_scripting("scripting")
   , _module_particles("particles")
  , _module_billboards("billboards")


{}

bool CEngine::start() {

    static CModuleLevel1 module_level_1("level_1");
    static CModuleLevel2 module_level_2("level_2");
    static CModuleTestInstancing module_test_instancing("test_instancing");


	_modules.registerSystemModule(&_module_render);
	_modules.registerSystemModule(&_module_entities);
	_modules.registerSystemModule(&_module_physics);
	_modules.registerSystemModule(&_module_ia);
	_modules.registerSystemModule(&_module_input);
	_modules.registerSystemModule(&_module_timer);
	_modules.registerSystemModule(&_module_tower);
	_modules.registerSystemModule(&_module_cameras);
	_modules.registerSystemModule(&_module_fsm);
	_modules.registerSystemModule(&_module_gui);
	_modules.registerSystemModule(&_module_sound);
    _modules.registerSystemModule(&_module_particles);
    _modules.registerSystemModule(&_module_billboards);

    _modules.registerGameModule(&module_level_1);
    _modules.registerGameModule(&module_level_2);
	_modules.registerGameModule(&module_test_instancing);

	_modules.loadModules("data/modules.json");
	_modules.loadGamestates("data/gamestates.json");

	return _modules.start();
}

bool CEngine::stop() {
	bool ok = true;
	ok &= _modules.stop();
	return ok;
}

void CEngine::update(float delta)
{
  PROFILE_FUNCTION("CEngine::update");
  current_unscaled_delta_time = delta;
  _modules.update(delta);
}

void CEngine::render()
{

	PROFILE_FUNCTION("CEngine::render");
	_module_render.generateFrame();
}
