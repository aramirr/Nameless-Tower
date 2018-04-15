#pragma once

#include "modules/module.h"
#include "render/mesh/mesh_instanced.h"

class CModuleTestInstancing : public IModule
{

  // -------------------------------------------------------------------
  struct TInstance {
    MAT44 world;
  };

  struct TInstanceBlood {
    MAT44 world;
    VEC4  color;
  };

  CRenderMeshInstanced* instances_mesh = nullptr;
  std::vector< TInstanceBlood > instances;

public:
  CModuleTestInstancing(const std::string& name)
		: IModule(name)
	{}
  bool start() override;
  void update(float delta) override;
};