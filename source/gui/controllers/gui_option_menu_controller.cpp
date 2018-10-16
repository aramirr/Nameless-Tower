#include "mcv_platform.h"
#include "gui_option_menu_controller.h"
#include "render/render_objects.h"
#include "gui/widgets/gui_button.h"

namespace GUI
{
	void COptionMenuController::update(float delta)
	{

		//-----------------------------------------------------------------------------------

		if (carga) {

			auto keyboardCB = []() {
				dbg("KEYBOARD \n");


			};

			auto fullScreenONCB = []() {
				dbg("FULLSCREEN ON\n");


			};

			auto fullScreenOFFCB = []() {
				dbg("FULLSCREEN OFF\n");

			};

			auto res1920CB = []() {
				dbg("RESOLUTION 1920\n");

			};

			auto res1366CB = []() {
				dbg("RESOLUTION 1366\n");

			};

			auto res1024CB = []() {
				dbg("RESOLUTION 1024\n");

			};

			auto graphicsLOWCB = []() {
				dbg("GRAPHICS LOW\n");

			};

			auto graphicsMEDIUMCB = []() {
				dbg("GRAPHICS MEDIUM\n");

			};

			auto graphicsHIGHCB = []() {
				dbg("GRAPHICS HIGH\n");

			};

			auto volumenLOWCB = []() {
				dbg("VOLUMEN LOW\n");

			};

			auto volumenMEDIUMCB = []() {
				dbg("VOLUMEN MEDIUM\n");

			};

			auto volumenHIGHCB = []() {
				dbg("VOLUMEN HIGH\n");

			};

			auto exitOptionsCB = []() {
				dbg("EXIT OPTIONS\n");

				exit(0);
			};

			registerOption("keyboard", keyboardCB, 0);
			registerOption("full_screen_ON", fullScreenONCB, 1);
			registerOption("full_screen_OFF", fullScreenOFFCB, 1);
			registerOption("res_1920", res1920CB, 2);
			registerOption("res_1366", res1366CB, 2);
			registerOption("res_1024", res1024CB, 2);
			registerOption("graphics_LOW", graphicsLOWCB, 3);
			registerOption("graphics_MEDIUM", graphicsMEDIUMCB, 3);
			registerOption("graphics_HIGH", graphicsHIGHCB, 3);
			registerOption("volumen_LOW", volumenLOWCB, 4);
			registerOption("volumen_MEDIUM", volumenMEDIUMCB, 4);
			registerOption("volumen_HIGH", volumenHIGHCB, 4);
			registerOption("exit_options", exitOptionsCB, 5);
			setCurrentSection(0);
			setCurrentOption(0);

			carga = false;
		}

		//-----------------------------------------------------------------------------------

		if (active) {
			if (EngineInput[VK_DOWN].getsPressed())
			{
				setCurrentSection(_currentSection + 1);
			}
			if (EngineInput[VK_UP].getsPressed())
			{
				setCurrentSection(_currentSection - 1);
			}
			if (EngineInput[VK_RIGHT].getsPressed())
			{
				setCurrentOption(_currentOption + 1);
			}
			if (EngineInput[VK_LEFT].getsPressed())
			{
				setCurrentOption(_currentOption - 1);
			}
			if (EngineInput[VK_RETURN].getsPressed())
			{
				_options[_currentSection][_currentOption].button->setCurrentState(CButton::EState::ST_Pressed);
			}
			if (EngineInput[VK_RETURN].getsReleased())
			{
				_options[_currentSection][_currentOption].button->setCurrentState(CButton::EState::ST_Selected);
				_options[_currentSection][_currentOption].callback();
			}
			if (EngineInput.mouse()._position_delta != VEC2(0, 0)) {
				getCurrentOption();
				setCurrentOption(_currentOption);
			}
			if (EngineInput["omnidash"].getsPressed()) {
				if (getCurrentOption() == _currentOption)
					_options[_currentSection][_currentOption].button->setCurrentState(CButton::EState::ST_Pressed);
			}
			if (EngineInput["omnidash"].getsReleased()) {
				if (getCurrentOption() == _currentOption) {
					_options[_currentSection][_currentOption].button->setCurrentState(CButton::EState::ST_Selected);
					_options[_currentSection][_currentOption].callback();
				}
			}
		}

	}

	void COptionMenuController::registerOption(const std::string& name, GUICallback cb, int i = 0)
	{
		CWidget* wdgt = EngineUI.getWidget(name, true);
		CButton* bt = dynamic_cast<CButton*>(wdgt);
		if (bt)
		{
			if (_options.size() == i) {
				_options.push_back(std::vector<TOption>());
			}
			_options[i].push_back(TOption({ bt, cb }));
		}
	}

	void COptionMenuController::setCurrentOption(int newOption)
	{
		for (auto& option : _options)
		{
			for (auto& _option : option)
			{
				_option.button->setCurrentState(CButton::EState::ST_Idle);
			}
		}

		_currentOption = clamp(newOption, 0, static_cast<int>(_options[_currentSection].size()) - 1);

		_options[_currentSection][_currentOption].button->setCurrentState(CButton::EState::ST_Selected);
	}

	void COptionMenuController::setCurrentSection(int newSection) {
		/*for (auto& option : _options)
		{
			for (auto& _option : option)
			{
				_option.button->setCurrentState(CButton::EState::ST_Idle);
			}
		}*/

		_currentSection = clamp(newSection, 0, static_cast<int>(_options.size()) - 1);
		setCurrentOption(0);

		//_options[_currentSection][0].button->setCurrentState(CButton::EState::ST_Selected);
	}

	void COptionMenuController::setActive(bool _active)
	{
		active = _active;
		if (active) {

			//PAUSAR JUEGO
			/*CEntity* player = (CEntity*)getEntityByName("The Player");

			TMsgSetFSMVariable pauseMsg;
			pauseMsg.variant.setName("pause");
			pauseMsg.variant.setBool(true);

			player->sendMsg(pauseMsg);*/

			EngineUI.activateWidget("menu_options");
			cb_gui.options = 1.f;
			EngineTimer.setTimeSlower(0.f);
		}
		else {
			cb_gui.options = 0.f;
			EngineUI.desactivateWidget("menu_options");
		}
	}

	int COptionMenuController::getCurrentOption()
	{
		int mX = EngineInput.mouse()._position.x;
		int mY = EngineInput.mouse()._position.y;
		for (int i = 0; i < _options.size(); i++) {
			for (int j = 0; j < _options[i].size(); j++) {
				int bmX = _options[i][j].button->getPosition().x;
				int bMX = bmX + _options[i][j].button->getSize().x;
				int bmY = _options[i][j].button->getPosition().y;
				int bMY = bmY + _options[i][j].button->getSize().y;
				if (mX >= bmX && mX <= bMX && mY >= bmY && mY <= bMY) {
					_currentSection = i;
					_currentOption = j;
					return j;
				}
			}
		}
		_currentOption = _currentOption;
		return -1;
	}
}
