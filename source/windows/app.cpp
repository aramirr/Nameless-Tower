#include "mcv_platform.h"
#include "app.h"
#include "render/render.h"

CApp* CApp::app_instance = nullptr;

// 
//--------------------------------------------------------------------------------------
LRESULT CALLBACK CApp::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
  PAINTSTRUCT ps;
  HDC hdc;

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
  xres = 640;
  yres = 480;
  return true;
}

//--------------------------------------------------------------------------------------
bool CApp::start() {
  if (!Render.createDevice( xres, yres ))
    return false;

  return true;
}

//--------------------------------------------------------------------------------------
bool CApp::stop() {
  Render.destroyDevice();
  return true;
}

//--------------------------------------------------------------------------------------
void CApp::doFrame() {
  
  Render.startRenderInBackbuffer();

  // Clear the back buffer 
  float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red,green,blue,alpha
  Render.ctx->ClearRenderTargetView(Render.renderTargetView, ClearColor);

  // Present the information rendered to the back buffer to the front buffer (the screen)
  Render.swapChain->Present(0, 0);
}

