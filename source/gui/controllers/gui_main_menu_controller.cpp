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
        //Engine.getModules().changeGameState("test_axis");
        EngineUI.desactivateWidget("pantallaInicio");
        EngineUI.desactiveMainMenu();

        CEntity* player = (CEntity*)getEntityByName("The Player");

        TMsgSetFSMVariable pauseMsg;
        pauseMsg.variant.setName("idle");
        pauseMsg.variant.setBool(true);

        player->sendMsg(pauseMsg);

        EngineTimer.setTimeSlower(1.f);
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
    if (EngineInput[VK_RETURN].getsPressed())
    {
      _options[_currentOption].button->setCurrentState(CButton::EState::ST_Pressed);
    }
    if (EngineInput[VK_RETURN].getsReleased())
    {
      _options[_currentOption].button->setCurrentState(CButton::EState::ST_Selected);
      _options[_currentOption].callback();
    }
		if (EngineInput.mouse()._position_delta != VEC2(0,0)) {
      getCurrentOption();
      setCurrentOption(_currentOption);
		}
		if (EngineInput["omnidash"].getsPressed()) {
			if(getCurrentOption() == _currentOption)
        _options[_currentOption].button->setCurrentState(CButton::EState::ST_Pressed);
		}
		if (EngineInput["omnidash"].getsReleased()) {
      if (getCurrentOption() == _currentOption) {
        _options[_currentOption].button->setCurrentState(CButton::EState::ST_Selected);
        _options[_currentOption].callback();
      }
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
  int CMainMenuController::getCurrentOption()
  {
    int mX = EngineInput.mouse()._position.x;
    int mY = EngineInput.mouse()._position.y;
    for (int i = 0; i < _options.size(); i++) {
      int bmX = _options[i].button->getPosition().x;
      int bMX = bmX + _options[i].button->getSize().x;
      int bmY = _options[i].button->getPosition().y;
      int bMY = bmY + _options[i].button->getSize().y;
      if (mX >= bmX && mX <= bMX && mY >= bmY && mY <= bMY) {
        _currentOption = i;
        return i;
      }
    }
    _currentOption = _currentOption;
    return -1;
  }
}
