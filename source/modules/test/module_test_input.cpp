#include "mcv_platform.h"
#include "module_test_input.h"

bool CModuleTestInput::start()
{
  return true;
}

void CModuleTestInput::update(float delta)
{
	float r = 0.f;
	float g = 0.f;
	float b = 0.f;

	const Input::TButton& bt = CEngine::get().getInput().host(Input::PLAYER_1).keyboard().key(VK_SPACE);
	r = EngineInput["red"].value;
	g = EngineInput["green"].value;
	b = 0.5f + 0.5f * EngineInput["blue"].value;

	CEngine::get().getRender().setBackgroundColor(r, g, b, 1.f);
}
