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

	float r = 0.f;
	float g = 0.f;
	float b = 0.f;

	//const Input::TButton& bt = CEngine::get().getInput().host(Input::PlayerId::PLAYER_1).keyboard().key(VK_SPACE);
	const Input::TButton& bt = EngineInput.button(Input::PAD_A);
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

	if (bt.isPressed())
	{
		/*const Input::TInterface_Mouse& mouse = EngineInput.mouse();
		dbg("MOUSE POSITION: %.2f, %.2f\n", mouse._position.x, mouse._position.y);*/
		auto& abt = EngineInput.button(Input::PAD_LANALOG_X);
		dbg("PAD LANALOG: %.8f\n", abt.value);
	}

	r = bt.value;
	g = EngineInput.button(Input::PAD_L2).value;
	b = EngineInput.button(Input::PAD_R2).value;
	//b = 0.5f + 0.5f * EngineInput.button(Input::PAD_LANALOG_X).value;

	/*Input::TInterface_Feedback feedback;
	feedback.leftRumble = g;
	feedback.rightRumble = b;
	EngineInput.feedback(feedback);*/

	if (EngineInput.button(Input::PAD_B).getsPressed())
	{
		Input::TInterface_Feedback feedback;
		feedback.leftRumble = 0.5f;
		feedback.rightRumble = 0.5f;
		EngineInput.feedback(feedback);
	}

	CEngine::get().getRender().setBackgroundColor(r, g, b, 1.f);
}