#include "mcv_platform.h"
#include "windows/app.h"
#include "render.h"

#pragma comment(lib, "d3d11")

CRender Render;

bool CRender::createDevice(int new_width, int new_height) {

  width = new_width;
  height = new_height;

  D3D_FEATURE_LEVEL featureLevels[] =
  {
    D3D_FEATURE_LEVEL_11_0,
  };
  UINT numFeatureLevels = ARRAYSIZE(featureLevels);

  UINT createDeviceFlags = 0;
#ifdef _DEBUG
  createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

  DXGI_SWAP_CHAIN_DESC sd;
  ZeroMemory(&sd, sizeof(sd));
  sd.BufferCount = 1;
  sd.BufferDesc.Width = width;
  sd.BufferDesc.Height = height;
  sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  sd.BufferDesc.RefreshRate.Numerator = 60;
  sd.BufferDesc.RefreshRate.Denominator = 1;
  sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  sd.OutputWindow = CApp::get().getWnd();
  sd.SampleDesc.Count = 1;
  sd.SampleDesc.Quality = 0;
  sd.Windowed = TRUE;

  D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
  HRESULT hr;

  hr = D3D11CreateDeviceAndSwapChain(
    NULL, D3D_DRIVER_TYPE_HARDWARE, NULL,
    createDeviceFlags, featureLevels, numFeatureLevels,
    D3D11_SDK_VERSION, &sd, &swapChain, &device, &featureLevel, &ctx);
  if (FAILED(hr))
    return false;

  // Create a render target view
  ID3D11Texture2D* pBackBuffer = NULL;
  hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
  if (FAILED(hr))
    return false;

  hr = device->CreateRenderTargetView(pBackBuffer, NULL, &renderTargetView);
  pBackBuffer->Release();
  if (FAILED(hr))
    return false;

  dbg("Render.Device created at %dx%d\n", width, height);

  return true;
}

// -------------------------------------------------------------------
void CRender::destroyDevice() {
  if (ctx) ctx->ClearState();
  SAFE_RELEASE(renderTargetView);
  SAFE_RELEASE(swapChain);
  SAFE_RELEASE(ctx);
  SAFE_RELEASE(device);
}

// -------------------------------------------------------------------
void CRender::startRenderInBackbuffer() {
  assert(ctx);

  ctx->OMSetRenderTargets(1, &renderTargetView, NULL);

  // Setup the viewport
  D3D11_VIEWPORT vp;
  vp.Width = (FLOAT)width;
  vp.Height = (FLOAT)height;
  vp.MinDepth = 0.0f;
  vp.MaxDepth = 1.0f;
  vp.TopLeftX = 0;
  vp.TopLeftY = 0;
  ctx->RSSetViewports(1, &vp);
}

