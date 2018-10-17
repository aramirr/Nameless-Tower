#pragma once

#include "comp_base.h"
#include "geometry/transform.h"


class TCompGrass : public TCompBase {
  DECL_SIBLING_ACCESS();
	
	struct grass_chunk {
		float width = 0.f;
		float length = 0.f;
		int total = 100;

		VEC3 pos1 = VEC3::Zero;
		VEC3 pos2 = VEC3::Zero;
		float radius1 = 0.f;
		float radius2 = 0.f;
	};

	std::vector<grass_chunk> grass_chunkes;

  

  bool carga = false;

public:
  void debugInMenu();
  void load(const json& j, TEntityParseContext& ctx);
  void update(float dt);

  void insert_all_grass();
	void insert_grass_by_id(int id);
	void delete_grass_by_id(int id);
};