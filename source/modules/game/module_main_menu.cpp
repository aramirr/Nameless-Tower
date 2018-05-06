#include "mcv_platform.h"
#include "module_main_menu.h"
#include "gui/controllers/gui_main_menu_controller.h"

bool CModuleMainMenu::start()
{
  CEngine::get().getRender().setBackgroundColor(0.f, 1.f, 0.f, 1.f);

  CModuleGUI& gui = Engine.getGUI();
  gui.getVariables().setVariant("progress", 0.5f);

  auto newGameCB = []() {
    printf("STARTING GAME\n");
  };
  auto continueCB = []() {
    printf("LOADING GAME\n");
  };
  auto optionsCB = []() {
    printf("CONFIGURING\n");
  };

  GUI::CMainMenuController* mmc = new GUI::CMainMenuController();
  mmc->registerOption("new_game", newGameCB);
  mmc->registerOption("continue", continueCB);
  mmc->registerOption("options", optionsCB);
  mmc->setCurrentOption(0);
  gui.registerController(mmc);

  gui.activateWidget("test");

  return true;
}

void CModuleMainMenu::update(float delta)
{
  if (isPressed(VK_SPACE))
  {
    CEngine::get().getModules().changeGameState("game_over");
  }


  // change bar value
  MVariants& guiVariables = Engine.getGUI().getVariables();
  float value = guiVariables.getFloat("progress");
  if (EngineInput[VK_LEFT].isPressed())
  {
    value = clamp(value - 0.5f * delta, 0.f, 1.f);
  }
  if (EngineInput[VK_RIGHT].isPressed())
  {
    value = clamp(value + 0.5f * delta, 0.f, 1.f);
  }
  guiVariables.setVariant("progress", value);
}