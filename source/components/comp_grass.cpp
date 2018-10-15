#include "mcv_platform.h"
#include "entity/entity_parser.h"
#include "comp_grass.h"
#include "juan/comp_transform.h"
#include "entity/common_msgs.h"

DECL_OBJ_MANAGER("comp_grass", TCompGrass);

void TCompGrass::debugInMenu() {}

void TCompGrass::load(const json& j, TEntityParseContext& ctx) {
	if (j.is_array()) {
		for (size_t i = 0; i < j.size(); ++i) {
			auto& j_grass = j[i];
			grass_chunk g;
			g.width = j_grass.value("width", 0.f);
			g.length = j_grass.value("length", 0.f);
			g.total = j_grass.value("total", 10);

			g.pos1 = loadVEC3(j_grass["pos1"]);
			g.pos2 = loadVEC3(j_grass["pos2"]);

			g.radius1 = j_grass.value("radius1", 0.f);
			g.radius2 = j_grass.value("radius2", 0.f);

			grass_chunkes.push_back(g);
		}
	}

}

void TCompGrass::insert_all_grass() {
	for (int i = 0; i < grass_chunkes.size(); ++i) {
		EngineBillboards.addGrassByAngle(grass_chunkes[i].pos1, grass_chunkes[i].pos2, grass_chunkes[i].total,
			grass_chunkes[i].radius1, grass_chunkes[i].radius2, i);
	}
}

void TCompGrass::insert_grass_by_id(int id) {
	EngineBillboards.addGrassByAngle(grass_chunkes[id].pos1, grass_chunkes[id].pos2, grass_chunkes[id].total, 
		grass_chunkes[id].radius1, grass_chunkes[id].radius2, id);
}


void TCompGrass::delete_grass_by_id(int id) {
	EngineBillboards.addGrassByAngle(grass_chunkes[id].pos1, grass_chunkes[id].pos2, grass_chunkes[id].total,
		grass_chunkes[id].radius1, grass_chunkes[id].radius2, id);
}



void TCompGrass::update(float dt) {
	if (!carga) {
		carga = true;
		insert_all_grass();
		std::vector<int> v;
		v.push_back(0);
		v.push_back(1);
		v.push_back(2);
		v.push_back(3);
		v.push_back(4);
		EngineBillboards.set_grass_ids_2_render(v);
		EngineBillboards.set_grass_instances_2_render();
	}
}



