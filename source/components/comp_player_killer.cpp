#include "mcv_platform.h"
#include "entity/entity_parser.h"
#include "comp_player_killer.h"
#include "juan/comp_transform.h"
#include "entity/common_msgs.h"

DECL_OBJ_MANAGER("player_killer", TCompPlayerKiller);

void TCompPlayerKiller::debugInMenu() {}

void TCompPlayerKiller::registerMsgs() {
	DECL_MSG(TCompPlayerKiller, TMsgKillPlayer, onTriggerEnter);
}

void TCompPlayerKiller::onTriggerEnter() {
	if (kill) {
		CEntity* player = (CEntity *)getEntityByName("The Player");
		TMsgSetFSMVariable deadMsg;
		deadMsg.variant.setName("hit");
		deadMsg.variant.setBool(true);
		player->sendMsg(deadMsg);
	}
}

bool TCompPlayerKiller::getKill() {
	return kill;
}

void TCompPlayerKiller::setKill(bool new_value) {
	kill = new_value;
}

void TCompPlayerKiller::load(const json& j, TEntityParseContext& ctx) {
  kill = j.value("kill", true);
}

void TCompPlayerKiller::update(float dt) {}

