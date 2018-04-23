#pragma once

#include "resources/resource.h"
#include "render/cte_buffer.h"
#include "ctes.h"                     // texture slots

class CTexture;

// ----------------------------------------------
class CMaterial : public IResource {

protected:
  bool  cast_shadows = true;

  static const int max_textures = TS_NUM_MATERIALS_SLOTS;
  CRenderCte<CCteMaterial> cb_material;

public:

  const CTexture* textures[max_textures];

  const CRenderTechnique* tech = nullptr;

  CMaterial();

  virtual void activate() const;
  virtual bool create(const json& j);
  void destroy() override;
  void debugInMenu() override;
  void onFileChanged(const std::string& filename) override;
  bool castsShadows() const { return cast_shadows; }

  void activateTextures(int slot) const;

protected:

  const ID3D11ShaderResourceView* srvs[max_textures];

};

// ----------------------------------------------
class CMaterialMixing : public CMaterial {
  const CMaterial*  mats[3] = { nullptr, nullptr, nullptr };
public:
  void activate() const override;
  bool create(const json& j) override;
  void debugInMenu() override;
};
