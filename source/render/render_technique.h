#pragma once

struct CRenderTechnique {
  CVertexShader *vs = nullptr;
  CPixelShader *ps = nullptr;
  void activate() {
    vs->activate();
    ps->activate();
  }
};

