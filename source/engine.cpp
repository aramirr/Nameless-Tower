#include "mcv_platform.h"
#include "engine.h"

#include "modules/game/module_splash.h"
#include "modules/game/module_main_menu.h"
#include "modules/game/module_gameover.h"


//--------------------------------------------------------------------------------------
CEngine& CEngine::get() {
  static CEngine engine;
  return engine;
}

CEngine::CEngine()
	: _module_render("render")
{}

bool CEngine::start() {
  static CModuleSplash module_splash("splash");
  static CModuleMainMenu module_main_menu("main_menu");
  static CModuleGameOver module_game_over("game_over");

	_modules.registerSystemModule(&_module_render);
	_modules.registerGameModule(&module_splash);
	_modules.registerGameModule(&module_main_menu);
	_modules.registerGameModule(&module_game_over);

  /*static CGameState gs_splash("splash");
  static CGameState gs_main_menu("main menu");
  static CGameState gs_game_over("game over");
  gs_splash.push_back(&module_splash);
  gs_main_menu.push_back(&module_main_menu);
  gs_game_over.push_back(&module_game_over);*/
  
  //_modules.registerGameState(&gs_splash);
  //_modules.registerGameState(&gs_main_menu);
  //_modules.registerGameState(&gs_game_over);

	_modules.loadModules("data/modules.json");
	_modules.loadGamestates("data/gamestates.json");

  bool ok = true;
  ok &= _modules.start();

  _modules.changeGameState("splash");

  return ok;
}

bool CEngine::stop() {
  bool ok = true;
  ok &= _modules.stop();
  return ok;
}

void CEngine::update(float delta)
{
  _modules.update(delta);
}

void CEngine::render()
{
  _modules.render();
}