#include "mcv_platform.h"
#include "app.h"
#include "render/render.h"
#include "input/devices/mouse.h"
#include "profiling/profiling.h"
#include <windowsx.h>
#include "render/render_objects.h"

CApp* CApp::app_instance = nullptr;

// 
//--------------------------------------------------------------------------------------
LRESULT CALLBACK CApp::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
  PAINTSTRUCT ps;
  HDC hdc;
	HBITMAP hImage;
	HDC hdcMem;

  // If the OS processes it, do not process anymore
  if (CEngine::get().getModules().OnOSMsg(hWnd, message, wParam, lParam))
    return 1;

  switch (message)
  {

  case CDirectoyWatcher::WM_FILE_CHANGED: {
    const char* filename = (const char*)lParam;
    dbg("File has changed! %s (%d)\n", filename, wParam);
    Resources.onFileChanged(filename);
    delete[] filename;
    break;
  }
  case WM_PAINT:
    // Validate screen repaint in os/windows 
		hdc = BeginPaint(hWnd, &ps);
    EndPaint(hWnd, &ps);
    break;

	case WM_ERASEBKGND:
		hdc = BeginPaint(hWnd, &ps);
		hImage = (HBITMAP)LoadImage(NULL, (LPCSTR)"data/textures/gui/carga.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_LOADTRANSPARENT);
		hdcMem = CreateCompatibleDC(hdc); // hDC is a DC structure supplied by Win32API
		SelectObject(hdcMem, hImage);
		StretchBlt(
			hdc,         // destination DC
			0,        // x upper left
			0,         // y upper left
			Render.width,       // destination width
			Render.height,      // destination height
			hdcMem,      // you just created this above
			0,
			0,          // x and y upper left
			1920,          // source bitmap width
			1080,          // source bitmap height
			SRCCOPY);   // raster operation
		EndPaint(hWnd, &ps);
		break;
  case WM_SETFOCUS:
    if (app_instance)
      app_instance->has_focus = true;
    break;

  case WM_KILLFOCUS:
    if (app_instance)
      app_instance->has_focus = false;
    break;

  case WM_DESTROY:
    PostQuitMessage(0);
    break;

	case WM_SIZE:
	{
		//dbg("aaaaaaa");
		//float xres = GetSystemMetrics(SM_CXSCREEN);
		//float yres = GetSystemMetrics(SM_CYSCREEN);

		//CEngine::get().getRender().configure(xres, yres);

		//if (Render.swapChain)
		//{
		//	Render.ctx->OMSetRenderTargets(0, 0, 0);

		//	// Release all outstanding references to the swap chain's buffers.
		//	Render.renderTargetView->Release();

		//	HRESULT hr;
		//	// Preserve the existing buffer count and format.
		//	// Automatically choose the width and height to match the client rect for HWNDs.
		//	hr = Render.swapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);

		//	// Perform error handling here!

		//	// Get buffer and create a render-target-view.
		//	ID3D11Texture2D* pBuffer;
		//	hr = Render.swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
		//		(void**)&pBuffer);
		//	// Perform error handling here!

		//	hr = Render.device->CreateRenderTargetView(pBuffer, NULL,
		//		&Render.renderTargetView);
		//	// Perform error handling here!
		//	pBuffer->Release();

		//	Render.ctx->OMSetRenderTargets(1, &Render.renderTargetView, NULL);

		//	// Set up the viewport.
		//	D3D11_VIEWPORT vp;
		//	vp.Width = (FLOAT)LOWORD(lParam);
		//	vp.Height = (FLOAT)HIWORD(lParam);
		//	vp.MinDepth = 0.0f;
		//	vp.MaxDepth = 1.0f;
		//	vp.TopLeftX = 0;
		//	vp.TopLeftY = 0;
		//	Render.ctx->RSSetViewports(1, &vp);

		//	CEngine::get().getRender().configure(LOWORD(lParam), HIWORD(lParam));
		//}
	}
	break;

	case WM_MOUSEMOVE:
	{
		Input::CMouse* mouse = static_cast<Input::CMouse*>(EngineInput.getDevice("mouse"));
		if (mouse)
		{
			int posX = GET_X_LPARAM(lParam);
			int posY = GET_Y_LPARAM(lParam);
			mouse->setPosition(static_cast<float>(posX), static_cast<float>(posY));
			app_instance->resetMouse = true;
		}
	}
		break;

	case WM_MBUTTONDOWN:
	{
		Input::CMouse* mouse = static_cast<Input::CMouse*>(EngineInput.getDevice("mouse"));
		if (mouse)
		{
			mouse->setButton(Input::MOUSE_MIDDLE, true);
			SetCapture(hWnd);
		}
	}
	break;

	case WM_MBUTTONUP:
	{
		Input::CMouse* mouse = static_cast<Input::CMouse*>(EngineInput.getDevice("mouse"));
		if (mouse)
		{
			mouse->setButton(Input::MOUSE_MIDDLE, false);
			ReleaseCapture();
		}
	}
	break;

	case WM_LBUTTONDOWN:
	{
		Input::CMouse* mouse = static_cast<Input::CMouse*>(EngineInput.getDevice("mouse"));
		if (mouse)
		{
			mouse->setButton(Input::MOUSE_LEFT, true);
			SetCapture(hWnd);
		}
	}
	break;

	case WM_LBUTTONUP:
	{
		Input::CMouse* mouse = static_cast<Input::CMouse*>(EngineInput.getDevice("mouse"));
		if (mouse)
		{
			mouse->setButton(Input::MOUSE_LEFT, false);
			ReleaseCapture();
		}
	}
	break;

	case WM_RBUTTONDOWN:
	{
		Input::CMouse* mouse = static_cast<Input::CMouse*>(EngineInput.getDevice("mouse"));
		if (mouse)
		{
			mouse->setButton(Input::MOUSE_RIGHT, true);
			SetCapture(hWnd);
		}
	}
	break;

	case WM_RBUTTONUP:
	{
		Input::CMouse* mouse = static_cast<Input::CMouse*>(EngineInput.getDevice("mouse"));
		if (mouse)
		{
			mouse->setButton(Input::MOUSE_RIGHT, false);
			ReleaseCapture();
		}
	}
	break;

	case WM_MOUSEWHEEL:
	{
		Input::CMouse* mouse = static_cast<Input::CMouse*>(EngineInput.getDevice("mouse"));
		if (mouse)
		{
			float wheel_delta = static_cast<float>(GET_WHEEL_DELTA_WPARAM(wParam)) / WHEEL_DELTA;
			mouse->setWheelDelta(wheel_delta);
		}
	}
	break;

  default:
    return DefWindowProc(hWnd, message, wParam, lParam);
  }

  return true;
}

//--------------------------------------------------------------------------------------
// Register class and create window
//--------------------------------------------------------------------------------------
bool CApp::createWindow(HINSTANCE new_hInstance, int nCmdShow) {

  hInstance = new_hInstance;

  // Register class
  WNDCLASSEX wcex;
  wcex.cbSize = sizeof(WNDCLASSEX);
  wcex.style = CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc = WndProc;
  wcex.cbClsExtra = 0;
  wcex.cbWndExtra = 0;
  wcex.hInstance = hInstance;
  wcex.hIcon = NULL;
  wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(NULL);//(COLOR_WINDOW + 1);
  wcex.lpszMenuName = NULL;
  wcex.lpszClassName = "MCVWindowsClass";
  wcex.hIconSm = NULL;
  if (!RegisterClassEx(&wcex))
    return false;

	DWORD dwStyle = (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU) ;

  // Create window
  RECT rc = { 0, 0, xres, yres };
  AdjustWindowRect(&rc, dwStyle, FALSE);
  hWnd = CreateWindow("MCVWindowsClass", "Direct3D 11 MCV Project",
		dwStyle,
    CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance,
    NULL);
  if (!hWnd)
    return false;

	/*HBITMAP hImage = (HBITMAP)LoadImage(NULL, "data/textures/gui/carga.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	HWND hImageView = CreateWindowEx(NULL, "STATIC", NULL, SS_BITMAP | WS_VISIBLE | WS_CHILD, 0, 0, 500, 600, hWnd, (HMENU)IMAGE_VIEW, GetModuleHandle(NULL), NULL);
	SendMessage(hImageView, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImage);*/

  ShowWindow(hWnd, nCmdShow);
	//ShowCursor(false);

  return true;
}

//--------------------------------------------------------------------------------------
// Process windows msgs, or if nothing to do, generate a new frame
//--------------------------------------------------------------------------------------
void CApp::mainLoop() {
  // Main message loop
  MSG msg = { 0 };
  while (WM_QUIT != msg.message)
  {
    // Check if windows has some msg for us
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    else
    {
      doFrame();

			/*if(resetMouse)
			{
				RECT rect;
				GetWindowRect(getWnd(), &rect);
				SetCursorPos((rect.left + rect.right) / 2, (rect.top + rect.bottom) / 2);
				resetMouse = false;
			}*/
    }
  }
}

//--------------------------------------------------------------------------------------
// Read any basic configuration required to boot, initial resolution, full screen, modules, ...
//--------------------------------------------------------------------------------------
bool CApp::readConfig() {
  // ...
  /*xres = 1366;
  yres = 768;*/

	/* xres = 1024;
  yres = 768;*/

	xres = 1920;
	yres = 1080;

  time_since_last_render.reset();

	cb_globals.global_first_resolution_X = xres;
	cb_globals.global_first_resolution_Y = yres;

  CEngine::get().getRender().configure(xres, yres);
  return true;
}

//--------------------------------------------------------------------------------------
bool CApp::start() {

  resources_dir_watcher.start("data", getWnd());

  return CEngine::get().start();
}

//--------------------------------------------------------------------------------------
bool CApp::stop() {
  return CEngine::get().stop();
}

//--------------------------------------------------------------------------------------
void CApp::doFrame() {
  PROFILE_FRAME_BEGINS();
  PROFILE_FUNCTION("App::doFrame");
  float dt = time_since_last_render.elapsedAndReset();
  CEngine::get().update(dt);
  CEngine::get().render();
}

