#include "mcv_platform.h"
#include "ui_mouse_pos.h"
#include "modules\module.h"
#include "entity\entity.h"
#include "render\render_utils.h"
#include "entity/entity_parser.h"

DECL_OBJ_MANAGER("arrow_ui", TCompArrowUI);

void TCompArrowUI::debugInMenu() {
}

void TCompArrowUI::load(const json& j, TEntityParseContext& ctx) {
	h_entity = ctx.current_entity;
}

void TCompArrowUI::update(float dt) {
	const Input::TInterface_Mouse& mouse = EngineInput.mouse();
	float action_button = EngineInput["action_button"].value;

	CEntity* e = h_entity;
	assert(e);
	TCompTransform *h_transform = e->get< TCompTransform >();
	assert(h_transform);
	VEC3 pos = h_transform->getPosition();

	if (available_capture == 1 && action_button == 1) {
		available_capture = 0;
		start_pos = mouse._position;
	}


	if (action_button == 1) {
		VEC2 move_vector = mouse._position - start_pos;
		move_vector.Normalize();
		dbg("MOUSE VECTOR: %.2f, %.2f\n", move_vector.x, move_vector.y * -1);
		dbg("World COORD VECTOR: %.2f, %.2f, %.2f\n", (pos.x + move_vector.x), (pos.y + (move_vector.y*-1)), (pos.z));
		unit_force_vector.x = pos.x + move_vector.x;
		unit_force_vector.y = pos.y + (move_vector.y*-1);
		unit_force_vector.z = pos.z;
	}
	
	if (action_button == 0 && available_capture == false) {
		available_capture = 1;
	}
}

