#include "mcv_platform.h"
#include "ctes.h"
#include "render/render_objects.h"
#include "gui_keyboard_menu_controller.h"
#include "gui/widgets/gui_button.h"

namespace GUI
{
  void CKeyboardMenuController::update(float delta)
  {

    //-----------------------------------------------------------------------------------

    if (carga) {

      auto exitKeyboardCB = []() {
        dbg("EXIT KEYBOARD\n");

				//EngineUI.desactivateWidget("keyboardMenu");
				EngineUI.desactiveKeyboardMenu();
				//EngineUI.activateWidget("menu_options");
				EngineUI.activeOptionMenu();
      };

      registerOption("keyboardExit", exitKeyboardCB);
      setCurrentOption(0);

      carga = false;
    }
		if (delta < 1.f)timer -= delta;
    //-----------------------------------------------------------------------------------

		if (active && timer <= 0.f) {
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

  void CKeyboardMenuController::registerOption(const std::string& name, GUICallback cb)
  {
    CWidget* wdgt = EngineUI.getWidget(name, true);
    CButton* bt = dynamic_cast<CButton*>(wdgt);
    if (bt)
    {
      _options.push_back(TOption({bt, cb}));
    }
  }

  void CKeyboardMenuController::setCurrentOption(int newOption)
  {
    for (auto& option : _options)
    {
      option.button->setCurrentState(CButton::EState::ST_Idle);
    }

    _currentOption = clamp(newOption, 0, static_cast<int>(_options.size()) - 1);

    _options[_currentOption].button->setCurrentState(CButton::EState::ST_Selected);
  }
	void CKeyboardMenuController::setActive(bool _active)
	{
		active = _active;
		if (active) {

			//PAUSAR JUEGO
			/*CEntity* player = (CEntity*)getEntityByName("The Player");

			TMsgSetFSMVariable pauseMsg;
			pauseMsg.variant.setName("pause");
			pauseMsg.variant.setBool(true);

			player->sendMsg(pauseMsg);*/
			timer = 0.3f;
			setCurrentOption(-1);
			EngineUI.activateWidget("keyboardMenu");
			EngineTimer.setTimeSlower(0.f);
		}
		else {
			EngineUI.desactivateWidget("keyboardMenu");
		}

	}

  int CKeyboardMenuController::getCurrentOption()
  {
		int mX = EngineInput.mouse()._position.x;
		int mY = EngineInput.mouse()._position.y;
		if (!cb_gui.fullscreen) {
			mX *= cb_globals.global_first_resolution_X / cb_globals.global_resolution_X;
			mY *= cb_globals.global_first_resolution_Y / cb_globals.global_resolution_Y;
		}
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
