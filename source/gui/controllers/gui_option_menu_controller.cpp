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
				dbg("KEYBOARD \n");


			};

			auto fullScreenONCB = []() {
				dbg("FULLSCREEN ON\n");
				Render.swapChain->SetFullscreenState(true, nullptr);

			};

			auto fullScreenOFFCB = []() {
				dbg("FULLSCREEN OFF\n");
				Render.swapChain->SetFullscreenState(false, nullptr);
			};

			auto res1920CB = []() {
				dbg("RESOLUTION 1920\n");
				
				HWND handle = ::FindWindowEx(0, 0, "MCVWindowsClass", 0);

				::SetWindowPos(handle, 0, 0, 0, 1920, 1080, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

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
					vp.Width = (FLOAT)1920;
					vp.Height = (FLOAT)1080;
					vp.MinDepth = 0.0f;
					vp.MaxDepth = 1.0f;
					vp.TopLeftX = 0;
					vp.TopLeftY = 0;
					Render.ctx->RSSetViewports(1, &vp);

					CEngine::get().getRender().configure(1920, 1080);
				}
			};

			auto res1366CB = []() {
				dbg("RESOLUTION 1366\n");
				
				HWND handle = ::FindWindowEx(0, 0, "MCVWindowsClass", 0);

				::SetWindowPos(handle, 0, 0, 0, 1366, 768, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

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
					vp.Width = (FLOAT)1366;
					vp.Height = (FLOAT)768;
					vp.MinDepth = 0.0f;
					vp.MaxDepth = 1.0f;
					vp.TopLeftX = 0;
					vp.TopLeftY = 0;
					Render.ctx->RSSetViewports(1, &vp);

					CEngine::get().getRender().configure(1366, 768);
				}
			};

			auto res1024CB = []() {
				dbg("RESOLUTION 1024\n");

				//DWORD dwStyle = (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME);

				HWND handle = ::FindWindowEx(0, 0, "MCVWindowsClass", 0);

				::SetWindowPos(handle, 0, 0, 0, 1024, 768, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
				//RedrawWindow(handle, NULL, NULL, RDW_INVALIDATE | RDW_NOERASE | RDW_INTERNALPAINT);
				//DefWindowProc(handle, uMsg, wParam, lParam);
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
					vp.Width = (FLOAT)1024;
					vp.Height = (FLOAT)768;
					vp.MinDepth = 0.0f;
					vp.MaxDepth = 1.0f;
					vp.TopLeftX = 0;
					vp.TopLeftY = 0;
					Render.ctx->RSSetViewports(1, &vp);

					CEngine::get().getRender().configure(1024, 768);
				}

				
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

				EngineUI.desactivateWidget("menu_options");
				EngineUI.desactiveOptionMenu();
				if (cb_gui.main > 0.f) EngineUI.activeMainMenu();
				else EngineUI.activePauseMenu();
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
