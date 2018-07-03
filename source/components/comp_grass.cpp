#include "mcv_platform.h"
#include "entity/entity_parser.h"
#include "comp_grass.h"
#include "juan/comp_transform.h"
#include "entity/common_msgs.h"

DECL_OBJ_MANAGER("comp_grass", TCompGrass);

void TCompGrass::debugInMenu() {}

void TCompGrass::load(const json& j, TEntityParseContext& ctx) {
	width = j.value("width", 0.f);
	length = j.value("length", 0.f);
	total = j.value("total", 10);

  pos1 = loadVEC3(j["pos1"]);
  pos2 = loadVEC3(j["pos2"]);
}

void TCompGrass::insert_grass() {
	CEntity* e = CHandle(this).getOwner();
	TCompTransform* my_transform = e->get<TCompTransform>();
  //EngineBillboards.addGrass(my_transform->getPosition(), width, length, total);
  EngineBillboards.addGrassByAngle(pos1, pos2, total);
}



void TCompGrass::update(float dt) {
	if (!carga) {
		carga = true;
		insert_grass();
	}
}



