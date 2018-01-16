#include "mcv_platform.h"
#include "module_render.h"
#include "windows/app.h"
#include "imgui/imgui_impl_dx11.h"

//--------------------------------------------------------------------------------------
CVertexShader vs;
CPixelShader ps;
CRenderMesh axis;
CRenderMesh triangle;

#include "render/cte_buffer.h"
#include "ctes.h"
CRenderCte<CCteCamera> cb_camera;


//--------------------------------------------------------------------------------------
bool CModuleRender::start()
{
  if (!Render.createDevice(_xres, _yres))
    return false;

  if (!CVertexDeclManager::get().create())
    return false;

  // --------------------------------------------
  // ImGui
  auto& app = CApp::get();
  if (!ImGui_ImplDX11_Init(app.getWnd(), Render.device, Render.ctx))
    return false;

  // --------------------------------------------
  // my custom code
  if (!vs.create("shaders.fx", "VS", "PosClr"))
    return false;

  if (!ps.create("shaders.fx", "PS"))
    return false;

  // --------------------------------------------
  // Axis aligned X,Y,Z of sizes 1,2,3
  float axis_vertices[] =
  {
    0.0f, 0.0f, 0.0f,  1, 0, 0, 1,
    1.0f, 0.0f, 0.0f,  1, 0, 0, 1,
    0.0f, 0.0f, 0.0f,  0, 1, 0, 1,
    0.0f, 2.0f, 0.0f,  0, 1, 0, 1,
    0.0f, 0.0f, 0.0f,  0, 0, 1, 1,
    0.0f, 0.0f, 3.0f,  0, 0, 1, 1,
  };
  if (!axis.create(axis_vertices, sizeof(axis_vertices), "PosClr", CRenderMesh::LINE_LIST))
    return false;

  float tri_vertices[] =
  {
    0.0f, 0.0f, 0.0f,  1, 0, 0, 1,
    1.0f, 0.0f, 0.0f,  0, 1, 0, 1,
    0.0f, 0.0f, 1.0f,  0, 0, 1, 1,
  };
  if (!triangle.create(tri_vertices, sizeof(tri_vertices), "PosClr", CRenderMesh::TRIANGLE_LIST))
    return false;

  // -------------------------------------------
  if (!cb_camera.create(CB_CAMERAS))
    return false;
  cb_camera.world = MAT44::Identity;

  VEC3 Eye = VEC3(2.0f, 2.0f, 3.0f);
  VEC3 At = VEC3(0.0f, 0.0f, 0.0f);
  VEC3 Up = VEC3(0.0f, 1.0f, 0.0f);
  cb_camera.view = MAT44::CreateLookAt(Eye, At, Up);
  cb_camera.proj = MAT44::CreatePerspectiveFieldOfView(75.f * (float)M_PI / 180.0f, (float)Render.width / (float)Render.height, 0.01f, 100.f);

  setBackgroundColor(0.0f, 0.125f, 0.3f, 1.f);

  return true;
}

// Forward the OS msg to the IMGUI
LRESULT CModuleRender::OnOSMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
  return ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
}

bool CModuleRender::stop()
{
  cb_camera.destroy();

  ps.destroy();
  vs.destroy();
  axis.destroy();
  triangle.destroy();

  ImGui_ImplDX11_Shutdown();

  Render.destroyDevice();
  return true;
}

void CModuleRender::update(float delta)
{
  // Notify ImGUI that we are starting a new frame
  ImGui_ImplDX11_NewFrame();
}

void CModuleRender::render()
{
  // Edit the Background color
  ImGui::ColorEdit4("Background Color", _backgroundColor);

  Render.startRenderInBackbuffer();

  // Clear the back buffer 
  float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red,green,blue,alpha
  Render.ctx->ClearRenderTargetView(Render.renderTargetView, _backgroundColor);

  cb_camera.updateGPU();
  cb_camera.activate();
  vs.activate();
  ps.activate();
  triangle.activateAndRender();

}

void CModuleRender::configure(int xres, int yres)
{
  _xres = xres;
  _yres = yres;
}

void CModuleRender::setBackgroundColor(float r, float g, float b, float a)
{
  _backgroundColor[0] = r;
  _backgroundColor[1] = g;
  _backgroundColor[2] = b;
  _backgroundColor[3] = a;
}

void CModuleRender::generateFrame() {

  CEngine::get().getModules().render();

  ImGui::Render();

  // Present the information rendered to the back buffer to the front buffer (the screen)
  Render.swapChain->Present(0, 0);
}

