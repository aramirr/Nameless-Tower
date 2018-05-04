#include "mcv_platform.h"
#include "entity/entity_parser.h"
#include "comp_trigger.h"
#include "components/juan/comp_transform.h"
#include "components/fsm/comp_fsm.h"
#include "components/ui/ui_mouse_pos.h"
#include "entity/common_msgs.h"
#include "modules\game\module_test_axis.h"

DECL_OBJ_MANAGER("trigger", TCompTrigger);

void TCompTrigger::debugInMenu() {
	
}

void TCompTrigger::onTriggerEnter(const TMsgTriggerEnter& msg) {
	h_other_entity = msg.h_other_entity;
	CEntity* e_other_entity = h_other_entity;
	std::string other_entity_name = e_other_entity->getName();
	if (trigger_type == "runner_appear" && other_entity_name == "The Player") {
		CEntity* runner_entity = (CEntity*)getEntityByName("Boss Runner");
		TMsgRunnerAppear msg2;
		msg2.b_appear = b_appear;
		msg2.next_state = next_state;
		if (b_appear)
			msg2.appearing_position = appearing_position;
		runner_entity->sendMsg(msg2);
	}

	else if (trigger_type == "runner_stop" && other_entity_name == "Boss Runner") {
		CEntity* runner_entity = (CEntity*)getEntityByName("Boss Runner");
		TMsgRunnerStop msg2;
		runner_entity->sendMsg(msg2);
	}
	else if (trigger_type == "checkpoint" && other_entity_name == "The Player") {
		TMsgCheckpoint msg2;
		msg2.appearing_position = appearing_position;
		e_other_entity->sendMsg(msg2);
		//CHandle(this).getOwner().destroy(); //En lugares con 2 caminos no puedes poner checkpoint a mitad del camino, solo en la interseccion entre los dos caminos (culpas a manu y leo)
	}
	else if (trigger_type == "plattform" && other_entity_name == "The Player"){
		CEntity* e_collider_entity = (CEntity*)getEntityByName(collider_entity);
		TMsgAttachTo attach_msg;
		attach_msg.h_attacher = h_other_entity;
		attach_msg.h_attached = e_collider_entity;
		e_collider_entity->sendMsg(attach_msg);
		CEntity* camDER = (CEntity *)getEntityByName("camera_manager");
		//CEntity* camIZQ = (CEntity *)getEntityByName("camera_orbit_IZQ");
		camDER->sendMsg(attach_msg);
		//camIZQ->sendMsg(attach_msg);
	}
	else if (trigger_type == "spikes" && other_entity_name == "The Player") {
		CEntity* player = (CEntity *)getEntityByName("The Player");
		TMsgSetFSMVariable deadMsg;
		deadMsg.variant.setName("hit");
		deadMsg.variant.setBool(true);		
		player->sendMsg(deadMsg);
	}
	else if (trigger_type == "player_killer" && other_entity_name == "The Player") {
		int a = EngineScripting.auxiliar_bdg;
		TMsgKillPlayer kill_player_message;
		CEntity * entity = h_entity;
		entity->sendMsg(kill_player_message);
	}
	else if (trigger_type == "destroyable" && other_entity_name == "The Player") {
		TMsgDestroy destroy_msg;
		CEntity* e_collider_entity = (CEntity*)getEntityByName(collider_entity);
		e_collider_entity->sendMsg(destroy_msg);
	}
	else if (trigger_type == "activador" && other_entity_name == "windstrike") {
		TMsgRotate rotate_msg;
		CEntity* e_collider_entity = (CEntity*)getEntityByName(collider_entity);
		e_collider_entity->sendMsg(rotate_msg);
	}
}

void TCompTrigger::onTriggerExit(const TMsgTriggerExit& msg) {
	h_other_entity = msg.h_other_entity;
	CEntity* e_other_entity = h_other_entity;
	std::string other_entity_name = e_other_entity->getName();
	if (trigger_type == "plattform" && other_entity_name == "The Player") {
		CEntity* e_collider_entity = (CEntity*)getEntityByName(collider_entity);
		TMsgDetachOf detach_msg;
		detach_msg.h_attacher = h_other_entity;
		detach_msg.h_attached = e_collider_entity;
		e_collider_entity->sendMsg(detach_msg);

		CEntity* camDER = (CEntity *)getEntityByName("camera_manager");
		//CEntity* camIZQ = (CEntity *)getEntityByName("camera_orbit_IZQ");
		camDER->sendMsg(detach_msg);
		//camIZQ->sendMsg(detach_msg);
	}

}

void TCompTrigger::registerMsgs() {
	DECL_MSG(TCompTrigger, TMsgTriggerEnter, onTriggerEnter);
	DECL_MSG(TCompTrigger, TMsgTriggerExit, onTriggerExit);
}

void TCompTrigger::load(const json& j, TEntityParseContext& ctx) {

	h_entity = ctx.current_entity;
	trigger_type = j.value("trigger_type", "none");

	render = j.value("render", true);
	if (trigger_type == "checkpoint")
	{
		appearing_position = loadVEC3(j["appearing_position"]);
	}
	else if (trigger_type == "runner_appear") {
		b_appear = j.value("b_appear", false);
		next_state = j.value("next_state", "chase");
		if (b_appear)
			appearing_position = loadVEC3(j["appearing_position"]);
	}
	collider_entity = j.value("collider_entity", "none");
	if (collider_entity == "none") {
		CEntity* e = h_entity;
		collider_entity = e->getName();
	}
	/*if (!render) {
		CEntity* e = h_entity;
		assert(e);
		h_render = e->get< TCompRender >();
		assert(h_render.isValid());
		TCompRender* r = h_render;
		r = h_render;
		TCompRender *my_render = getMyRender();
		my_render->is_active = false;
	}*/
}

void TCompTrigger::update(float dt) {};


