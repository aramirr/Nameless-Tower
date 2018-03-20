#pragma once

#include "resources/resource.h"
#include "texture_slots.h"

class CTexture;

// ----------------------------------------------
class CMaterial : public IResource {
  bool  cast_shadows = true;

public:

  const CTexture* textures[TS_NUM_MATERIALS_SLOTS];

  const CRenderTechnique* tech = nullptr;

  void activate() const;
  bool create(const std::string& name);
  void debugInMenu() override;
  void onFileChanged(const std::string& filename) override;
  bool castsShadows() const { return cast_shadows; }


protected:

  const ID3D11ShaderResourceView* srvs[TS_COUNT];

};

