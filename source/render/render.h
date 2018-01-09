#ifndef INC_RENDER_H_
#define INC_RENDER_H_

class CRender {

public:
  ID3D11Device*           device = NULL;
  ID3D11DeviceContext*    ctx = NULL;
  ID3D11RenderTargetView* renderTargetView = NULL;
  IDXGISwapChain*         swapChain = NULL;

  int width = 0;
  int height = 0;

  bool createDevice(int new_width, int new_height);
  void destroyDevice();
  void startRenderInBackbuffer();
};

extern CRender Render;

#endif

