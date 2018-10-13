#include "mcv_platform.h"
#include "entity/entity_parser.h"
#include "comp_render_windstrike.h"
#include "../juan/comp_transform.h"
#include "entity/common_msgs.h"

DECL_OBJ_MANAGER("render_windstrike", TCompRenderWindstrike);

void TCompRenderWindstrike::debugInMenu() {}

void TCompRenderWindstrike::load(const json& j, TEntityParseContext& ctx) {
	srand(time(NULL));
	id = rand();
}

void TCompRenderWindstrike::generate_windstrike_sprite() {
	CEntity* e = CHandle(this).getOwner();
	TCompTransform* my_transform = e->get<TCompTransform>();
	id = EngineBillboards.addWindstrike(my_transform->getPosition());
}



void TCompRenderWindstrike::update(float dt) {
	if (!carga) {
		carga = true;
		CEntity* e = CHandle(this).getOwner();
		TCompTransform* my_transform = e->get<TCompTransform>();
		id = EngineBillboards.addWindstrike(my_transform->getPosition());
	}
	else {
		CEntity* e = CHandle(this).getOwner();
		if (e) {
			TCompTransform* my_transform = e->get<TCompTransform>();
			EngineBillboards.moveWindstrike(my_transform->getPosition(), id);
		}
		else {
			EngineBillboards.deleteWindstrike(id);
		}
	}
}



