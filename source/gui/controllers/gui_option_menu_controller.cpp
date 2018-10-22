#include "mcv_platform.h"
#include "gui_option_menu_controller.h"
#include "render/render_objects.h"
#include "gui/widgets/gui_button.h"
#include <windowsx.h>

namespace GUI
{
	void COptionMenuController::update(float delta)
	{

		//-----------------------------------------------------------------------------------

		if (carga) {

			auto keyboardCB = []() {
				//dbg("KEYBOARD \n");

				EngineUI.activeKeyboardMenu();
				EngineUI.desactiveOptionMenu();
				cb_gui.options = 1.f;
			};

			auto fullScreenONCB = []() {
				//dbg("FULLSCREEN ON\n");
				EngineUI.fullScreen(true);
				EngineUI.changeResolution(Render.width, Render.height);
				EngineUI.fullScreen(true);
				EngineUI.setFullScreenOption(1, 0);
				//fullScreenOption = VEC2(1, 0);
			};

			auto fullScreenOFFCB = []() {
				//dbg("FULLSCREEN OFF\n");
				EngineUI.fullScreen(false);
				EngineUI.changeResolution(Render.width, Render.height);
				EngineUI.fullScreen(false);
				EngineUI.setFullScreenOption(1, 1);
			};

			auto res1920CB = []() {
				//dbg("RESOLUTION 1920\n");
				if ((Render.width != 1920 && Render.height != 1080) && 
					(1920 <= cb_globals.global_first_resolution_X && 1080 <= cb_globals.global_first_resolution_Y)) {
					EngineUI.changeResolution(1920, 1080);
					if (cb_gui.fullscreen) EngineUI.fullScreen(true);
					EngineUI.setResolutionOption(2, 0);
				}	
			};

			auto res1600CB = []() {
				//dbg("RESOLUTION 1600\n");
				if ((Render.width != 1600 && Render.height != 900) &&
					(1600 <= cb_globals.global_first_resolution_X && 900 <= cb_globals.global_first_resolution_Y)) {
					EngineUI.changeResolution(1600, 900);
					if (cb_gui.fullscreen) EngineUI.fullScreen(true);
					EngineUI.setResolutionOption(2, 1);
				}
			};

			auto res12809CB = []() {
				//dbg("RESOLUTION 12809\n");
				if ((Render.width != 1280 && Render.height != 960) &&
					(1280 <= cb_globals.global_first_resolution_X && 960 <= cb_globals.global_first_resolution_Y)) {
					EngineUI.changeResolution(1280, 960);
					if (cb_gui.fullscreen) EngineUI.fullScreen(true);
					EngineUI.setResolutionOption(2, 2);
				}
			};

			auto res1366CB = []() {
				//dbg("RESOLUTION 1920\n");
				if ((Render.width != 1366 && Render.height != 768) &&
					(1366 <= cb_globals.global_first_resolution_X && 768 <= cb_globals.global_first_resolution_Y)) {
					EngineUI.changeResolution(1366, 768);
					if (cb_gui.fullscreen) EngineUI.fullScreen(true);
					EngineUI.setResolutionOption(3, 0);
				}
			};

			auto res12807CB = []() {
				//dbg("RESOLUTION 1366\n");
				if ((Render.width != 1280 && Render.height != 720) &&
					(1280 <= cb_globals.global_first_resolution_X && 720 <= cb_globals.global_first_resolution_Y)) {
					EngineUI.changeResolution(1280, 720);
					if (cb_gui.fullscreen) EngineUI.fullScreen(true);
					EngineUI.setResolutionOption(3, 1);
				}
			};

			auto res1024CB = []() {
				//dbg("RESOLUTION 1024\n");
				if ((Render.width != 1024 && Render.height != 768) &&
					(1024 <= cb_globals.global_first_resolution_X && 768 <= cb_globals.global_first_resolution_Y)) {
					EngineUI.changeResolution(1024, 768);
					if (cb_gui.fullscreen) EngineUI.fullScreen(true);
					EngineUI.setResolutionOption(3, 2);
				}
			};

			auto volumenOFFCB = []() {
				//dbg("VOLUMEN LOW\n");
				EngineSound.setVolumen(0.f);
				EngineUI.setVolumenOption(4, 0);
			};


			auto volumenLOWCB = []() {
				//dbg("VOLUMEN LOW\n");
				EngineSound.setVolumen(0.3f);
				EngineUI.setVolumenOption(4, 1);
			};

			auto volumenMEDIUMCB = []() {
				//dbg("VOLUMEN MEDIUM\n");
				EngineSound.setVolumen(0.6f);
				EngineUI.setVolumenOption(4, 2);
			};

			auto volumenHIGHCB = []() {
				//dbg("VOLUMEN HIGH\n");
				EngineSound.setVolumen(1.f);
				EngineUI.setVolumenOption(4, 3);
			};

			auto vsyncONCB = []() {
				//dbg("VSYNC ON\n");
				cb_gui.vsync = true;
				EngineUI.setVsyncOption(5, 0);
			};

			auto vsyncOFFCB = []() {
				//dbg("VSYNC OFF\n");
				cb_gui.vsync = false;
				EngineUI.setVsyncOption(5, 1);
			};

			auto exitOptionsCB = []() {
				//dbg("EXIT OPTIONS\n");

				//EngineUI.desactivateWidget("menu_options");
				EngineUI.desactiveOptionMenu();
				if (cb_gui.main > 0.f) EngineUI.activeMainMenu();
				else EngineUI.activePauseMenu();
			};

			registerOption("keyboard", keyboardCB, 0);
			registerOption("full_screen_ON", fullScreenONCB, 1);
			registerOption("full_screen_OFF", fullScreenOFFCB, 1);
			registerOption("res_1920", res1920CB, 2);
			registerOption("res_1600", res1600CB, 2);
			registerOption("res_12809", res12809CB, 2);
			registerOption("res_1366", res1366CB, 3);
			registerOption("res_12807", res12807CB, 3);
			registerOption("res_1024", res1024CB, 3);
			//registerOption("graphics_LOW", graphicsLOWCB, 3);
			//registerOption("graphics_MEDIUM", graphicsMEDIUMCB, 3);
			//registerOption("graphics_HIGH", graphicsHIGHCB, 3);
			registerOption("volumen_OFF", volumenOFFCB, 4);
			registerOption("volumen_LOW", volumenLOWCB, 4);
			registerOption("volumen_MEDIUM", volumenMEDIUMCB, 4);
			registerOption("volumen_HIGH", volumenHIGHCB, 4);
			registerOption("vsyncON", vsyncONCB, 5);
			registerOption("vsyncOFF", vsyncOFFCB, 5);
			registerOption("exit_options", exitOptionsCB, 6);
			setCurrentSection(0);
			setCurrentOption(0);

			fullScreenOption = VEC2(1, 1);
			resolutionOption = VEC2(2, 0);
			volumenOption = VEC2(4, 3);
			vsyncOption = VEC2(5, 1);

			cb_gui.fullscreen = false;
			cb_gui.vsync = false;
			cb_gui.keyboard = false;

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
		int i = 0;
		int j = 0;
		for (auto& option : _options)
		{
			j = 0;
			for (auto& _option : option)
			{
				VEC2 cOption = VEC2(i, j);
				if(cOption == fullScreenOption || cOption == resolutionOption || cOption == volumenOption || cOption == vsyncOption)_option.button->setCurrentState(CButton::EState::ST_Selected);
				else _option.button->setCurrentState(CButton::EState::ST_Idle);
				j++;
			}
			i++;
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

			setCurrentSection(0);
			setCurrentOption(0);

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
		//dbg("%f - %f\n", (float)mX, (float)mY);
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
	void COptionMenuController::changeResolution(int _x, int _y)
	{

		//EngineUI.getCamera().setOrthographic(_x, _y);

		HWND handle = ::FindWindowEx(0, 0, "MCVWindowsClass", 0);

		if(!cb_gui.fullscreen) ::SetWindowPos(handle, 0, 0, 0, _x, _y, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
		else ::SetWindowPos(handle, 0, 0, 0, _x, _y, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE /*| SWP_NOSIZE*/);

		if (Render.swapChain)
		{
			Render.ctx->OMSetRenderTargets(0, 0, 0);

			// Release all outstanding references to the swap chain's buffers.
			Render.renderTargetView->Release();

			HRESULT hr;
			// Preserve the existing buffer count and format.
			// Automatically choose the width and height to match the client rect for HWNDs.
			hr = Render.swapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);

			// Perform error handling here!

			// Get buffer and create a render-target-view.
			ID3D11Texture2D* pBuffer;
			hr = Render.swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
				(void**)&pBuffer);
			// Perform error handling here!

			hr = Render.device->CreateRenderTargetView(pBuffer, NULL,
				&Render.renderTargetView);
			// Perform error handling here!
			pBuffer->Release();

			Render.ctx->OMSetRenderTargets(1, &Render.renderTargetView, NULL);

			// Set up the viewport.
			D3D11_VIEWPORT vp;
			vp.Width = (FLOAT)_x;
			vp.Height = (FLOAT)_y;
			vp.MinDepth = 0.0f;
			vp.MaxDepth = 1.0f;
			/*if(cb_gui.fullscreen) vp.TopLeftX = (GetSystemMetrics(SM_CXSCREEN) - _x)/2;
			else*/ vp.TopLeftX = 0;
			vp.TopLeftY = 0;
			Render.ctx->RSSetViewports(1, &vp);

			CEngine::get().getRender().configure(_x, _y);
		}

	}
	void COptionMenuController::fullScreen(bool _fulscreen)
	{
		HWND handle = ::FindWindowEx(0, 0, "MCVWindowsClass", 0);
		bool isChangeSuccessful;
		if (_fulscreen) {
			cb_gui.fullscreen = true;
			DEVMODE fullscreenSettings;
			
			RECT windowBoundary;

			HDC windowHDC = GetDC(handle);
			int fullscreenWidth = GetDeviceCaps(windowHDC, HORZRES);
			int fullscreenHeight = GetDeviceCaps(windowHDC, VERTRES);
			int colourBits = GetDeviceCaps(windowHDC, BITSPIXEL);
			int refreshRate = GetDeviceCaps(windowHDC, VREFRESH);
			//dbg("----------------------FrameRate: %f\n", (float)refreshRate);
			EnumDisplaySettings(NULL, 0, &fullscreenSettings);
			fullscreenSettings.dmPelsWidth = fullscreenWidth;
			fullscreenSettings.dmPelsHeight = fullscreenHeight;
			fullscreenSettings.dmBitsPerPel = colourBits;
			fullscreenSettings.dmDisplayFrequency = refreshRate;
			fullscreenSettings.dmFields = DM_PELSWIDTH |
				DM_PELSHEIGHT |
				DM_BITSPERPEL |
				DM_DISPLAYFREQUENCY;

			SetWindowLongPtr(handle, GWL_EXSTYLE, WS_EX_APPWINDOW | WS_EX_TOPMOST);
			SetWindowLongPtr(handle, GWL_STYLE, WS_POPUP | WS_VISIBLE);
			SetWindowPos(handle, HWND_TOPMOST, 0, 0, fullscreenWidth, fullscreenHeight, SWP_SHOWWINDOW);
			isChangeSuccessful = ChangeDisplaySettings(&fullscreenSettings, CDS_FULLSCREEN) == DISP_CHANGE_SUCCESSFUL;
			ShowWindow(handle, SW_MAXIMIZE);

			//ESTO AGRANDA UI CEngine::get().getRender().configure(fullscreenWidth, fullscreenHeight);
		}
		else if(!_fulscreen)
		{
			cb_gui.fullscreen = false;
			SetWindowLongPtr(handle, GWL_EXSTYLE, WS_EX_LEFT);
			SetWindowLongPtr(handle, GWL_STYLE, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU);
			isChangeSuccessful = ChangeDisplaySettings(NULL, CDS_RESET) == DISP_CHANGE_SUCCESSFUL;
			SetWindowPos(handle, HWND_NOTOPMOST, 0, 0, cb_globals.global_resolution_X, cb_globals.global_resolution_Y, SWP_SHOWWINDOW);
			ShowWindow(handle, SW_RESTORE);
		}

	}
	void COptionMenuController::setFullScreenOption(int x, int y)
	{
		fullScreenOption = VEC2(x, y);
	}

	void COptionMenuController::setResolutionOption(int x, int y)
	{
		resolutionOption = VEC2(x, y);
	}

	void COptionMenuController::setVolumenOption(int x, int y)
	{
		volumenOption = VEC2(x, y);
	}

	void COptionMenuController::setVsyncOption(int x, int y)
	{
		vsyncOption = VEC2(x, y);
	}
}
