#include "mcv_platform.h"
#include "entity/entity_parser.h"
#include "comp_compresora_kill.h"
#include "entity/common_msgs.h"

DECL_OBJ_MANAGER("comp_compressora_kill", TCompCompresoraKill);

void TCompCompresoraKill::debugInMenu() {}

void TCompCompresoraKill::checkIfTouchingPlayer(const TMsgCheckPlayerIn& msg) {
  if (player_in) {
    CEntity *player = (CEntity *)getEntityByName("The Player");
    TMsgSetFSMVariable deadMsg;
    deadMsg.variant.setName("hit");
    deadMsg.variant.setBool(true);
    player->sendMsg(deadMsg);
  }
}

void TCompCompresoraKill::playerIn(const TMsgPlayerIn& msg) {
  player_in = true;
  TMsgCheckPlayerIn msg2;
  CEntity* e = getEntityByName(other_name);
  e->sendMsg(msg2);
}

void TCompCompresoraKill::playerOut(const TMsgPlayerOut& msg) {
  player_in = false;
}

void TCompCompresoraKill::registerMsgs() {
  DECL_MSG(TCompCompresoraKill, TMsgCheckPlayerIn, checkIfTouchingPlayer);
  DECL_MSG(TCompCompresoraKill, TMsgPlayerIn, playerIn);
  DECL_MSG(TCompCompresoraKill, TMsgPlayerOut, playerOut);
}

void TCompCompresoraKill::load(const json& j, TEntityParseContext& ctx) {
	other_name = j.value("other_name", "");
}

void TCompCompresoraKill::update(float dt) {}



