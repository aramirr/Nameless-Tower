#pragma once

#include "modules/module.h"
#include "render/mesh/mesh_instanced.h"

class CModuleBillboards : public IModule
{

  // -------------------------------------------------------------------
  struct TInstance {
    MAT44 world;
  };
  CRenderMeshInstanced* instances_mesh = nullptr;
  std::vector< TInstance > instances;

  // -------------------------------------------------------------------
  struct TInstanceBlood {
    MAT44 world;
    VEC4  color;
  };
  CRenderMeshInstanced* blood_instances_mesh = nullptr;
  std::vector< TInstanceBlood > blood_instances;

  // -------------------------------------------------------------------
  struct TRenderParticle {
    VEC3  pos;      // Maps to iCenter.xyz
    float angle;    // Maps to iCenter.w
    VEC4  color;
    float scale_x;
    float scale_y;
    float nframe;
	int id;
  };
  CRenderMeshInstanced* particles_instances_mesh = nullptr;
  std::vector< TRenderParticle > particles_instances;


  // -------------------------------------------------------------------
  struct TGrassParticle {
    VEC3  pos;
  };
  CRenderMeshInstanced* grass_instances_mesh = nullptr;
  std::vector< TGrassParticle > grass_instances;

public:
	int max_id = 0;

  CModuleBillboards (const std::string& name)
		: IModule(name)
	{}
  bool start() override;
  void update(float delta) override;
  int addFuegoTest(VEC3 position, float scale);
  void apagarFuego(int id);
  void encenderFuego(int id, float scale);
  void addGrass(VEC3 position, float width, float length, int total);
  void addGrassByAngle(VEC3 pos1, VEC3 pos2, int total);
  void calculateAngles(VEC3 pos1, VEC3 pos2, float& alpha, float& beta, float& charlie);
};