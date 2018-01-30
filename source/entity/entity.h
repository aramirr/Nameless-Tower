#pragma once

#include "handle/handle.h"
#include "components/comp_base.h"

// 1 entity son 128 handles => 128x4 => 512bytes

class CEntity : public TCompBase {

  CHandle comps[CHandle::max_types];

public:

  CHandle get(uint32_t comp_type) const {
    assert(comp_type < CHandle::max_types);
    return comps[comp_type];
  }
  void debugInMenu();

  void set(uint32_t comp_type, CHandle new_comp);

};


// Forward declarations
class CRenderMesh;
class CTexture;
class CRenderTechnique;
class IAIController;

struct TEntity {
  std::string  name;
  CTransform   transform;
  const CRenderMesh* mesh = nullptr;
  const CTexture* texture = nullptr;
  CRenderTechnique* tech = nullptr;
  IAIController* ai_controller = nullptr;
  
  void load(const json& j);
};

