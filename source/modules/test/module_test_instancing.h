#pragma once

#include "modules/module.h"
#include "render/mesh/mesh_instanced.h"

class CRenderMeshInstancedBase;

class CModuleTestInstancing : public IModule
{

  // -------------------------------------------------------------------
  struct TRenderDecal3D {
    MAT44 world;
    VEC4  color;
    TRenderDecal3D()
      : color(1, 1, 1, 1)
    {}
    static const char* getDeclName() {
      return "DecalInstance";
    }
  };

  typedef CRenderMeshInstanced< TRenderDecal3D > TInstancedDecals;

  TInstancedDecals* mesh = nullptr;

public:
  CModuleTestInstancing(const std::string& name)
		: IModule(name)
	{}
  bool start() override;
  void update(float delta) override;
};