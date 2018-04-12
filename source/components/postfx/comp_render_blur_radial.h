#ifndef INC_COMPONENT_RENDER_BLUR_RADIAL_H_
#define INC_COMPONENT_RENDER_BLUR_RADIAL_H_

#include "components/comp_base.h"
#include <vector>

struct CBlurStep;
class  CTexture;
class CRenderToTexture;

// ------------------------------------
struct TCompRenderBlurRadial : public TCompBase {
  CRenderToTexture*   rt_output = nullptr;
  const CRenderTechnique* tech = nullptr;
  const CRenderMesh*      mesh = nullptr;

  VEC4  weights;
  VEC4  distance_factors;     // 1 2 3 4
  int   xres, yres;
  float global_distance;
  bool  enabled;
  void  load(const json& j, TEntityParseContext& ctx);
  void  debugInMenu();
  CTexture* apply(CTexture* in_texture);
};

#endif
