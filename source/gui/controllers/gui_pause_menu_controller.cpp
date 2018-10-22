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

			auto restartCB = []() {
				dbg("RESTART\n");
				
			};

			auto optionsCB = []() {
				dbg("OPTIONS\n");
				cb_gui.options = 1.f;
				EngineUI.activateWidget("menu_options");
				EngineUI.activeOptionMenu();
				EngineUI.desactivePauseMenu();
			};

      auto exitCB = []() {

        exit(0);
      };

      registerOption("resume_game", resumeGameCB);
			registerOption("restart_level", restartCB);
			registerOption("options_pause", optionsCB);
      registerOption("exit_pause", exitCB);
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
		HWND handle = ::FindWindowEx(0, 0, "MCVWindowsClass", 0);
    if (active) {

      //PAUSAR JUEGO
      /*CEntity* player = (CEntity*)getEntityByName("The Player");

      TMsgSetFSMVariable pauseMsg;
      pauseMsg.variant.setName("pause");
      pauseMsg.variant.setBool(true);

      player->sendMsg(pauseMsg);*/

			

			HCURSOR Cursor = LoadCursorFromFile("data/textures/gui/PaiCursor.cur"); //.cur or .ani
			SetCursor(Cursor);
			SetClassLongPtr(handle, GCLP_HCURSOR, reinterpret_cast<LONG_PTR>(Cursor));

      EngineUI.activateWidget("menu_pausa");
			cb_gui.pause = 1.f;
      EngineTimer.setTimeSlower(0.f);
    }
    else {
			if (cb_gui.pause == 1.f && cb_gui.options == 1.f && cb_gui.keyboard == 1.f ) {
				HCURSOR Cursor = LoadCursorFromFile("data/textures/gui/cursorIngame.cur"); //.cur or .ani
				SetCursor(Cursor);
				SetClassLongPtr(handle, GCLP_HCURSOR, reinterpret_cast<LONG_PTR>(Cursor));
			}

			cb_gui.pause = 0.f;
      EngineUI.desactivateWidget("menu_pausa");
    }
  }

	void CPauseMenuController::resetOptions()
	{
		setCurrentOption(0);
	}

  int CPauseMenuController::getCurrentOption()
  {
		int mX = EngineInput.mouse()._position.x;
		int mY = EngineInput.mouse()._position.y;
		if (cb_gui.fullscreen) {
			//mX *= cb_globals.global_first_resolution_X / cb_globals.global_resolution_X;
			//mY *= cb_globals.global_first_resolution_Y / cb_globals.global_resolution_Y;

			mX *= 1920 / cb_globals.global_first_resolution_X;
			mY *= 1080 / cb_globals.global_first_resolution_Y;
		}
		else {
			mX *= 1920 / cb_globals.global_resolution_X;
			mY *= 1080 / cb_globals.global_resolution_Y;
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
