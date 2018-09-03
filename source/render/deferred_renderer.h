#ifndef INC_RENDER_DEFERRED_RENDERER_H_
#define INC_RENDER_DEFERRED_RENDERER_H_

#include "render/texture/render_to_texture.h"

class CRenderToCube;

class CDeferredRenderer {

public:
  
  CRenderToTexture* rt_normals = nullptr;
  CRenderToTexture* rt_albedos = nullptr;;
	CRenderToTexture* rt_depth = nullptr;;
	CRenderToTexture* rt_self_illum = nullptr;;
	CRenderToTexture* rt_cell = nullptr;;
	CRenderToTexture* rt_acc_light = nullptr;;
	CRenderToTexture* rt_alpha= nullptr;;
//  CRenderToTexture* rt_sublime = nullptr;;

  void renderGBuffer();
  void renderGBufferDecals();
  void renderAccLight();
  void renderAmbientPass();
  void renderDirectionalLights();
  void renderPointLights();
	void renderPointLightsShadows();
  void renderSkyBox() const;
  void renderAO(CHandle h_camera) const;

public:

  bool create( int xres, int yres, const char* prefix);
  void render( CRenderToTexture* rt_destination, CHandle h_camera);
	void renderToCubeFace(CRenderToCube* rt_destination, int face_idx);

};


#endif

