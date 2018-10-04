#include "mcv_platform.h"
#include "entity/entity_parser.h"
#include "comp_player_killer.h"
#include "../juan/comp_transform.h"
#include "entity/common_msgs.h"

DECL_OBJ_MANAGER("player_killer", TCompPlayerKiller);

void TCompPlayerKiller::debugInMenu() {}

void TCompPlayerKiller::registerMsgs() {
	DECL_MSG(TCompPlayerKiller, TMsgKillPlayer, onTriggerEnter);
}

void TCompPlayerKiller::onTriggerEnter(const TMsgKillPlayer& msg) {
	if (kill_enabled) {
		CEntity* player = (CEntity *)getEntityByName("The Player");
		TCompCollider* player_collider = player->get<TCompCollider>();
		player_collider->config.mask = CModulePhysics::FilterGroup::Scenario;
		EngineUI.desactivateWidget("text_pista");
		EngineUI.desactivateWidget("text_sala_eleccion");
		EngineUI.desactivateWidget("text_plataforma_activable");
		EngineUI.desactivateWidget("text_planear");
		TMsgSetFSMVariable deadMsg;
		deadMsg.variant.setName("hit");
		deadMsg.variant.setBool(true);
		//player->sendMsg(deadMsg);
	}
}

bool TCompPlayerKiller::getKill() {
	return kill_enabled;
}

void TCompPlayerKiller::setKill(bool new_value) {
	kill_enabled = new_value;
}

void TCompPlayerKiller::load(const json& j, TEntityParseContext& ctx) {
	kill_enabled = j.value("kill_enabled", true);
}

void TCompPlayerKiller::update(float dt) {}

