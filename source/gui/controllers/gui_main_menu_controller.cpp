#include "mcv_platform.h"
#include "gui_main_menu_controller.h"
#include "gui/widgets/gui_button.h"

namespace GUI
{
  void CMainMenuController::update(float delta)
  {

    //-----------------------------------------------------------------------------------

    if (carga) {
      CEntity* player = (CEntity*)getEntityByName("The Player");

      TMsgSetFSMVariable pauseMsg;
      pauseMsg.variant.setName("pause");
      pauseMsg.variant.setBool(true);

      player->sendMsg(pauseMsg);

      auto newGameCB = []() {
        dbg("STARTING GAME\n");
        Engine.getModules().changeGameState("test_axis");
        EngineUI.desactivateWidget("pantallaInicio");
        EngineUI.desactiveMainMenu();

        CEntity* player = (CEntity*)getEntityByName("The Player");

        TMsgSetFSMVariable pauseMsg;
        pauseMsg.variant.setName("idle");
        pauseMsg.variant.setBool(true);

        player->sendMsg(pauseMsg);
      };
      auto exitCB = []() {
        dbg("LOADING GAME\n");

        exit(0);
      };

      registerOption("new_game", newGameCB);
      registerOption("exit", exitCB);
      setCurrentOption(0);

      carga = false;
    }

    //-----------------------------------------------------------------------------------


    if (EngineInput[VK_DOWN].getsPressed())
    {
      setCurrentOption(_currentOption + 1);
    }
    if (EngineInput[VK_UP].getsPressed())
    {
      setCurrentOption(_currentOption - 1);
    }
    if (EngineInput[VK_SPACE].getsPressed())
    {
      _options[_currentOption].button->setCurrentState(CButton::EState::ST_Pressed);
    }
    if (EngineInput[VK_SPACE].getsReleased())
    {
      _options[_currentOption].button->setCurrentState(CButton::EState::ST_Selected);
      _options[_currentOption].callback();
    }
  }

  void CMainMenuController::registerOption(const std::string& name, GUICallback cb)
  {
    CWidget* wdgt = EngineUI.getWidget(name, true);
    CButton* bt = dynamic_cast<CButton*>(wdgt);
    if (bt)
    {
      _options.push_back(TOption({bt, cb}));
    }
  }

  void CMainMenuController::setCurrentOption(int newOption)
  {
    for (auto& option : _options)
    {
      option.button->setCurrentState(CButton::EState::ST_Idle);
    }

    _currentOption = clamp(newOption, 0, static_cast<int>(_options.size()) - 1);

    _options[_currentOption].button->setCurrentState(CButton::EState::ST_Selected);
  }
}
