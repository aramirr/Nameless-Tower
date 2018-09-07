#include "mcv_platform.h"
#include "entity/entity_parser.h"
#include "comp_trigger.h"
#include "components/juan/comp_transform.h"
#include "components/fsm/comp_fsm.h"
#include "components/ui/ui_mouse_pos.h"
#include "entity/common_msgs.h"

DECL_OBJ_MANAGER("trigger", TCompTrigger);

void TCompTrigger::debugInMenu() {
	
}

void TCompTrigger::onTriggerEnter(const TMsgTriggerEnter& msg) {
	CEntity* entity = h_entity;
	std::string name = entity->getName();

	std::vector<std::string> params;
	params.push_back(name);

	h_other_entity = msg.h_other_entity;
	CEntity* e_other_entity = h_other_entity;
	std::string other_entity_name = e_other_entity->getName();

	if (other_entity_name == "The Player") {
		EngineScripting.ExecEvent(ScriptEvents::trigger_enter, params);

		if (trigger_type == "checkpoint") {
      if (!appear_on_transform_position)
			  Engine.getTower().setLastCheckpoint(appearing_position);
      else {
        TCompTransform *entity_transform = entity->get<TCompTransform>();
        Engine.getTower().setLastCheckpoint(entity_transform->getPosition());
      }
		}
		else if (trigger_type == "plattform") {
			CEntity* e_collider_entity = (CEntity*)getEntityByName(collider_entity);
			TMsgAttachTo attach_msg;
			attach_msg.h_attacher = h_other_entity;
			attach_msg.h_attached = e_collider_entity;
			e_collider_entity->sendMsg(attach_msg);
			CEntity* camDER = (CEntity *)getEntityByName("camera_manager");
			camDER->sendMsg(attach_msg);
		}
		else if (trigger_type == "player_killer") {

			TMsgKillPlayer kill_player_message;
			entity->sendMsg(kill_player_message);
		}
		else if (trigger_type == "destroyable") {
			TMsgDestroy destroy_msg;
			CEntity* e_collider_entity = (CEntity*)getEntityByName(collider_entity);
			e_collider_entity->sendMsg(destroy_msg);
		}
		else if (trigger_type == "runner_appear") {
			TMsgRunnerAppear appear_msg;
			appear_msg.appearing_position = appearing_position;
			CEntity* e_collider_entity = (CEntity*)getEntityByName("Runner");
			e_collider_entity->sendMsg(appear_msg);
		}
    else if (trigger_type == "compresora") {
      CEntity* e = h_entity;
      TMsgPlayerIn msg_in;
      e->sendMsg(msg_in);
    }
	}
	else if (other_entity_name == "windstrike") {
		EngineScripting.ExecEvent(ScriptEvents::windstrike_activate, params);
		if (trigger_type == "activador" && other_entity_name == "windstrike") {
			TMsgRotate rotate_msg;
			CEntity* e_collider_entity = (CEntity*)getEntityByName(collider_entity);
			e_collider_entity->sendMsg(rotate_msg);
		}
		else if (trigger_type == "torch" && other_entity_name == "windstrike") {
			TMsgDeactivateTorch deactivate_msg;
      TMsgDestroy destroy_msg;
      e_other_entity->sendMsg(destroy_msg);
			CEntity* e_collider_entity = (CEntity*)getEntityByName(collider_entity);
			e_collider_entity->sendMsg(deactivate_msg);
		}
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
		camDER->sendMsg(detach_msg);
	}
  else if (trigger_type == "compresora" && other_entity_name == "The Player") {
    CEntity* e = h_entity;
    TMsgPlayerOut msg_out;
    e->sendMsg(msg_out);
  }

}

void TCompTrigger::registerMsgs() {
	DECL_MSG(TCompTrigger, TMsgTriggerEnter, onTriggerEnter);
	DECL_MSG(TCompTrigger, TMsgTriggerExit, onTriggerExit);
}

void TCompTrigger::load(const json& j, TEntityParseContext& ctx) {

	h_entity = ctx.current_entity;
	trigger_type = j.value("trigger_type", "none");

	if (trigger_type == "checkpoint" or trigger_type == "runner_appear") {
    if (j.count("appearing_position")) {
      appearing_position = loadVEC3(j["appearing_position"]);
      appear_on_transform_position = false;
    }
    else {
      appear_on_transform_position = true;
    }
	}

	collider_entity = j.value("collider_entity", "none");
	if (collider_entity == "none") {
		CEntity* e = h_entity;
		collider_entity = e->getName();
	}
}

void TCompTrigger::update(float dt) {};


