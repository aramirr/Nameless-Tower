#include "mcv_platform.h"
#include "app.h"
#include "render/render.h"

CApp* CApp::app_instance = nullptr;

// 
//--------------------------------------------------------------------------------------
LRESULT CALLBACK CApp::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
  PAINTSTRUCT ps;
  HDC hdc;

  // If the OS processes it, do not process anymore
  if (CEngine::get().getModules().OnOSMsg(hWnd, message, wParam, lParam))
    return 1;

  switch (message)
  {
  case WM_PAINT:
    // Validate screen repaint in os/windows 
    hdc = BeginPaint(hWnd, &ps);
    EndPaint(hWnd, &ps);
    break;

  case WM_DESTROY:
    PostQuitMessage(0);
    break;

  default:
    return DefWindowProc(hWnd, message, wParam, lParam);
  }

  return 0;
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
  wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
  wcex.lpszMenuName = NULL;
  wcex.lpszClassName = "MCVWindowsClass";
  wcex.hIconSm = NULL;
  if (!RegisterClassEx(&wcex))
    return false;

  // Create window
  RECT rc = { 0, 0, xres, yres };
  AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
  hWnd = CreateWindow("MCVWindowsClass", "Direct3D 11 MCV Project",
    WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance,
    NULL);
  if (!hWnd)
    return false;

  ShowWindow(hWnd, nCmdShow);

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
    }
  }
}

//--------------------------------------------------------------------------------------
// Read any basic configuration required to boot, initial resolution, full screen, modules, ...
//--------------------------------------------------------------------------------------
bool CApp::readConfig() {
  // ...
  xres = 1024;
  yres = 640;

  CEngine::get().getRender().configure(xres, yres);
  return true;
}

//--------------------------------------------------------------------------------------
bool CApp::start() {
  return CEngine::get().start();
}

//--------------------------------------------------------------------------------------
bool CApp::stop() {
  return CEngine::get().stop();
}

//--------------------------------------------------------------------------------------
void CApp::doFrame() {
  static const float frameTime = 1.f / 60.f;
  CEngine::get().update(frameTime);
  CEngine::get().render();
  Sleep(static_cast<DWORD>(frameTime * 1000.f));
}

