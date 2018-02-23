#include "mcv_platform.h"
#include "entity/entity_parser.h"
#include "comp_trigger.h"
#include "comp_transform.h"
#include "ui/ui_mouse_pos.h"
#include "entity/common_msgs.h"

DECL_OBJ_MANAGER("trigger", TCompTrigger);

void TCompTrigger::debugInMenu() {
	
}

void TCompTrigger::onTriggerEnter(const TMsgTriggerEnter& msg) {
	h_other_entity = msg.h_other_entity;
	CEntity* e_other_entity = h_other_entity;
	std::string other_entity_name = e_other_entity->getName();
	if (trigger_type == "runner" && other_entity_name == "The Player") {
		CEntity* tunner_entity = (CEntity*)getEntityByName("Boss Runner");
		TMsgAppear msg2;
		msg2.appearing_position = appearing_position;
		tunner_entity->sendMsg(msg2);
	}
}

void TCompTrigger::registerMsgs() {
	DECL_MSG(TCompTrigger, TMsgTriggerEnter, onTriggerEnter);
}


void TCompTrigger::load(const json& j, TEntityParseContext& ctx) {
	trigger_type = j.value("trigger_type", "none");
	appearing_position = loadVEC3(j["appearing_position"]);
}

void TCompTrigger::update(float dt) {};


