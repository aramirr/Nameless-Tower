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
	if (trigger_type == "plattform_orbit" && other_entity_name == "The Player"){
		CEntity* e_current_entity = h_entity;
		TMsgAttachTo attach_msg;
		attach_msg.h_attacher = h_other_entity;
		attach_msg.h_attached = h_entity;
		e_other_entity->sendMsg(attach_msg);
	}
}
void TCompTrigger::onTriggerExit(const TMsgTriggerExit& msg) {
	h_other_entity = msg.h_other_entity;
	CEntity* e_other_entity = h_other_entity;
	std::string other_entity_name = e_other_entity->getName();
	if (trigger_type == "plattform_orbit" && other_entity_name == "The Player") {
		CEntity* e_current_entity = h_entity;
		TMsgDetachOf detach_msg;
		detach_msg.h_attacher = h_other_entity;
		detach_msg.h_attached = h_entity;
		e_other_entity->sendMsg(detach_msg);
	}
}


void TCompTrigger::registerMsgs() {
	DECL_MSG(TCompTrigger, TMsgTriggerEnter, onTriggerEnter);
	DECL_MSG(TCompTrigger, TMsgTriggerExit, onTriggerExit);
}


void TCompTrigger::load(const json& j, TEntityParseContext& ctx) {
	h_entity = ctx.current_entity;
	trigger_type = j.value("trigger_type", "none");
	if (trigger_type == "runner")
	{
		appearing_position = loadVEC3(j["appearing_position"]);
	}
}

void TCompTrigger::update(float dt) {};


