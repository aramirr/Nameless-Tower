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
  };
	CRenderMeshInstanced* particles_instances_mesh = nullptr;
    CRenderMeshInstanced* fire_particles_instances_mesh = nullptr;
    CRenderMeshInstanced* fire_particles_violeta_instances_mesh = nullptr;
    CRenderMeshInstanced* fire_particles_azul_instances_mesh = nullptr;
    CRenderMeshInstanced* thin_fire_particles_instances_mesh = nullptr;
    CRenderMeshInstanced* thin_fire_violeta_particles_instances_mesh = nullptr;
    CRenderMeshInstanced* thin_fire_azul_particles_instances_mesh = nullptr;
    CRenderMeshInstanced* thin_fire_azul_apagando_particles_instances_mesh = nullptr;
    CRenderMeshInstanced* smoke_4_prendiendo_particles_instances_mesh = nullptr;
	CRenderMeshInstanced* smoke_particles_instances_mesh = nullptr;
	CRenderMeshInstanced* thin_smoke_particles_instances_mesh = nullptr;
	std::vector< TRenderParticle > particles_instances;
    std::vector< TRenderParticle > fire_particles_instances;
    std::vector< TRenderParticle > fire_azul_particles_instances;
    std::vector< TRenderParticle > fire_violeta_particles_instances;
    std::vector< TRenderParticle > thin_fire_particles_instances;
    std::vector< TRenderParticle > thin_fire_violeta_particles_instances;
    std::vector< TRenderParticle > thin_fire_azul_particles_instances;
    std::vector< TRenderParticle > thin_fire_azul_apagando_particles_instances;
    std::vector< TRenderParticle > smoke_4_prendiendo_particles_instances;
    std::vector< TRenderParticle > smoke_particles_instances;
    std::vector< TRenderParticle > thin_smoke_particles_instances;
	std::vector< int > particles_ids;
    std::vector< int > fire_particles_ids;
    std::vector< int > fire_violeta_particles_ids;
    std::vector< int > fire_azul_particles_ids;
    std::vector< int > thin_fire_particles_ids;
    std::vector< int > thin_fire_violeta_particles_ids;
    std::vector< int > thin_fire_azul_particles_ids;
    std::vector< int > thin_fire_azul_apagando_particles_ids;
    std::vector< int > smoke_prendiendo_particles_ids;
    std::vector< int > smoke_particles_ids;
    std::vector< int > thin_smoke_particles_ids;


  // -------------------------------------------------------------------
  struct TGrassParticle {
    VEC3  pos;
  };
	CRenderMeshInstanced* grass_instances_mesh = nullptr;
	CRenderMeshInstanced* grass_bajada_instances_mesh = nullptr;
  std::vector< TGrassParticle > grass_instances;
	std::vector< std::vector<TGrassParticle>> grass_intances_vector;
	std::vector<int> grass_vectors_2_render;
	std::vector< TGrassParticle > grass_instances_2_render;

	



  // -------------------------------------------------------------------
  struct TWindstrikeParticle {
	  VEC3  pos;      // Maps to iCenter.xyz
	  float angle;    // Maps to iCenter.w
	  VEC4  color;
	  float scale_x;
	  float scale_y;
	  float nframe;
  };
  CRenderMeshInstanced* windstrike_instances_mesh = nullptr;
  std::vector< TWindstrikeParticle > windstrike_instances;
  std::vector< int > windstrike_ids;


public:
	int fuego_max_id = 0;
	int windstrike_max_id = 0;

  CModuleBillboards (const std::string& name)
		: IModule(name)
	{}
  bool start() override;
  void update(float delta) override;
  int addFuegoTest(VEC3 position, float scale, bool thin);
  int addFuegoVioleta(VEC3 position, float scale, bool thin);
  int addFuegoAzul(VEC3 position, float scale, float smoke_y_offset);
  void apagarFuego(int id, float scale, bool thin);
  void apagarFuegoAzul(int id, float scale, VEC3 position, float y_offset);
  void apagandoFuegoAzul(int id, float scale, VEC3 position, float y_offset);
  void prendiendoHumo(int id, float scale, VEC3 position, float y_offset);
  void encenderFuego(int id, float scale, bool thin);
  void encenderFuegoVioleta(int id, float scale, bool thin);
  void encenderFuegoAzul(int id, float scale);
  void addGrass(VEC3 position, float width, float length, int total);
  void addGrassByAngle(VEC3 pos1, VEC3 pos2, int total, float radius1, float radius2, int id);
  void calculateAngles(VEC3 pos1, VEC3 pos2, float& alpha, float& beta, float& charlie);
  int addWindstrike(VEC3 position);
  void clearFire();
  void deleteWindstrike(int id);
  void moveWindstrike(VEC3 position, int id);

	void set_grass_instances_2_render();
	void set_grass_ids_2_render(std::vector<int>& ids);

	void add_grass_id(int id);
	void delete_grass_id(int id);
};