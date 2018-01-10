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
CVertexShader vs;
CPixelShader ps;
CRenderMesh triangle;

#include "geometry/geometry.h"
#include "render/cte_buffer.h"
#include "ctes.h"
CRenderCte<CCteCamera> cb_camera;

//--------------------------------------------------------------------------------------
bool CApp::start() {
  if (!Render.createDevice( xres, yres ))
    return false;

  if (!CVertexDeclManager::get().create())
    return false;

  // my custom code
  if (!vs.create("shaders.fx", "VS", "PosClr"))
    return false;

  if (!ps.create("shaders.fx", "PS"))
    return false;

  float vertices[] =
  {
    0.0f, 0.0f, 0.0f,  1, 0, 0, 1,
    0.0f, 0.0f, 1.0f,  0, 1, 0, 1,
    1.0f, 0.0f, 0.0f,  0, 0, 1, 1,
  };
  if (!triangle.create(vertices, sizeof(vertices), "PosClr", CRenderMesh::TRIANGLE_LIST))
    return false;

  // -------------------------------------------
  if (!cb_camera.create(CB_CAMERAS))
    return false;
  cb_camera.world = MAT44::Identity;

  VEC3 Eye = VEC3(3.0f, 4.0f, 5.0f);
  VEC3 At = VEC3(0.0f, 0.0f, 0.0f);
  VEC3 Up = VEC3(0.0f, 1.0f, 0.0f);
  cb_camera.view = MAT44::CreateLookAt(Eye, At, Up);
  cb_camera.proj = MAT44::CreatePerspectiveFieldOfView(M_PI * 0.5f, 1.0f, 0.01f, 100.f);

  return true;
}

//--------------------------------------------------------------------------------------
bool CApp::stop() {

  cb_camera.destroy();

  ps.destroy();
  vs.destroy();
  triangle.destroy();

  Render.destroyDevice();
  return true;
}

//--------------------------------------------------------------------------------------
void CApp::doFrame() {
  
  Render.startRenderInBackbuffer();

  // Clear the back buffer 
  float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red,green,blue,alpha
  Render.ctx->ClearRenderTargetView(Render.renderTargetView, ClearColor);

  cb_camera.updateGPU();
  cb_camera.activate();
  vs.activate();
  ps.activate();
  triangle.activateAndRender();

  // Present the information rendered to the back buffer to the front buffer (the screen)
  Render.swapChain->Present(0, 0);
}

