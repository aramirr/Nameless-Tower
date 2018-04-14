#pragma once

#include "modules/module.h"
#include "render/mesh/mesh_instanced.h"

class CRenderMeshInstancedBase;

class CModuleTestInstancing : public IModule
{

  // -------------------------------------------------------------------
  struct TInstance {
    MAT44 world;
    static const char* getDeclName() {
      return "Instance";
    }
  };

  typedef CRenderMeshInstanced< TInstance > TInstancedMeshes;

  TInstancedMeshes* mesh = nullptr;

public:
  CModuleTestInstancing(const std::string& name)
		: IModule(name)
	{}
  bool start() override;
  void update(float delta) override;
};