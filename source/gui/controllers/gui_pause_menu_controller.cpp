#include "mcv_platform.h"
#include "gui_pause_menu_controller.h"
#include "render/render_objects.h"
#include "gui/widgets/gui_button.h"

namespace GUI
{
  void CPauseMenuController::update(float delta)
  {

    //-----------------------------------------------------------------------------------

    if (carga) {

      auto resumeGameCB = []() {
        EngineTimer.setTimeSlower(1.f);
        //Engine.getModules().changeGameState("test_axis");
        EngineUI.desactivateWidget("menu_pausa");
        EngineUI.desactivePauseMenu();

        CEntity* player = (CEntity*)getEntityByName("The Player");

        TMsgSetFSMVariable pauseMsg;
        pauseMsg.variant.setName("idle");
        pauseMsg.variant.setBool(true);

        player->sendMsg(pauseMsg);
        
      };
      auto exitCB = []() {

        exit(0);
      };

      registerOption("resume_game", resumeGameCB);
      registerOption("exit_2", exitCB);
      setCurrentOption(0);

      carga = false;
    }

    //-----------------------------------------------------------------------------------

    if (active) {
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
      if (EngineInput.mouse()._position_delta != VEC2(0, 0)) {
        getCurrentOption();
        setCurrentOption(_currentOption);
      }
      if (EngineInput["omnidash"].getsPressed()) {
        if (getCurrentOption() == _currentOption)
          _options[_currentOption].button->setCurrentState(CButton::EState::ST_Pressed);
      }
      if (EngineInput["omnidash"].getsReleased()) {
        if (getCurrentOption() == _currentOption) {
          _options[_currentOption].button->setCurrentState(CButton::EState::ST_Selected);
          _options[_currentOption].callback();
        }
      }
    }
    
  }

  void CPauseMenuController::registerOption(const std::string& name, GUICallback cb)
  {
    CWidget* wdgt = EngineUI.getWidget(name, true);
    CButton* bt = dynamic_cast<CButton*>(wdgt);
    if (bt)
    {
      _options.push_back(TOption({bt, cb}));
    }
  }

  void CPauseMenuController::setCurrentOption(int newOption)
  {
    for (auto& option : _options)
    {
      option.button->setCurrentState(CButton::EState::ST_Idle);
    }

    _currentOption = clamp(newOption, 0, static_cast<int>(_options.size()) - 1);

    _options[_currentOption].button->setCurrentState(CButton::EState::ST_Selected);
  }
  void CPauseMenuController::setActive(bool _active)
  {
    active = _active;
    if (active) {

      //PAUSAR JUEGO
      /*CEntity* player = (CEntity*)getEntityByName("The Player");

      TMsgSetFSMVariable pauseMsg;
      pauseMsg.variant.setName("pause");
      pauseMsg.variant.setBool(true);

      player->sendMsg(pauseMsg);*/

      EngineUI.activateWidget("menu_pausa");
			cb_gui.pause = 1.f;
      EngineTimer.setTimeSlower(0.f);
    }
    else {
			cb_gui.pause = 0.f;
      EngineUI.desactivateWidget("menu_pausa");
    }
  }

  int CPauseMenuController::getCurrentOption()
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
