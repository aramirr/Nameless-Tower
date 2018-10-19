#include "mcv_platform.h"
#include "comp_destoyable_son.h"
#include "render/render_objects.h"
#include "render/render_utils.h"
#include "../juan/comp_transform.h"
#include "../juan/comp_render.h"
#include "../juan/comp_group.h"
#include "../lights/comp_light_spot.h"
#include "render/texture/material.h"
#include "ctes.h" 
#include "entity/entity_parser.h"
#include "components/juan/comp_name.h"

DECL_OBJ_MANAGER("destoyable_son", TCompDestroyableSon);

void TCompDestroyableSon::debugInMenu() {

}

void TCompDestroyableSon::load(const json& j, TEntityParseContext& ctx) {
	h_entity = ctx.current_entity;

	father_name = j.value("father", "");
	
}

/* Update the values during the given time */
void TCompDestroyableSon::update(float dt) {
	if (carga) {
		carga = false;
		CEntity* e = h_entity;

		CEntity* f = (CEntity *)getEntityByName(father_name);

		TMsgRegisterDestoyableSon msg;
		std::string n = e->getName();
		msg.name = n;
		f->sendMsg(msg);
	}

}

