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

DECL_OBJ_MANAGER("destoyable_son", TCompDestoyableSon);

void TCompDestoyableSon::debugInMenu() {

}

void TCompDestoyableSon::load(const json& j, TEntityParseContext& ctx) {

	std::string father = j.value("father", "-");
	if (father == "-") assert(false);
	std::string me = j.value("me", "-");
	if (me == "-") assert(false);
	CEntity* f = (CEntity *)getEntityByName(father);
	assert(f);
	TMsgRegisterDestoyableSon msg;
	msg.name = me;
	f->sendMsg(msg);
}

/* Update the values during the given time */
void TCompDestoyableSon::update(float dt) {


}

