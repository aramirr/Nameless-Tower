#pragma once

#include "handle/handle.h"
#include "components/comp_base.h"

class CEntity : public TCompBase {

  CHandle comps[CHandle::max_types];

public:

  CHandle get(uint32_t comp_type) const {
    assert(comp_type < CHandle::max_types);
    return comps[comp_type];
  }

  template< typename TComp >
  CHandle get() const {
    auto om = getObjectManager<TComp>();
    assert(om);
    return comps[om->getType()];
  }

  void debugInMenu();

  void set(uint32_t comp_type, CHandle new_comp);
  void load(const json& j, TEntityParseContext& ctx);

  const char* getName() const;

};


