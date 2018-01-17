#include "mcv_platform.h"
#include "module_splash.h"

bool CModuleSplash::start()
{
  _timer = 3.f;
  CEngine::get().getRender().setBackgroundColor(1.f, 0.f, 0.f, 1.f);
  return true;
}

void CModuleSplash::update(float delta)
{
  /*_timer -= delta;
  if (_timer <= 0.f)
  {
    CEngine::get().getModules().changeGameState("main_menu");
  }*/

	const Input::TButton& bt = CEngine::get().getInput().host(Input::PlayerId::PLAYER_1).keyboard().key(VK_SPACE);
	//const Input::TButton& bt = CEngine::get().getInput().mapped("next_screen");
	if (bt.getsReleased())
	{
		dbg("BUTTON RELEASED: %.2f\n", bt.time_pressed);
		//CEngine::get().getModules().changeGameState("main_menu");
	}
	if (bt.getsPressed())
	{
		dbg("BUTTON PRESSED: %.2f\n", bt.time_released);
		//CEngine::get().getModules().changeGameState("main_menu");
	}
}