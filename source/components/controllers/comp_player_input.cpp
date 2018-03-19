#include "mcv_platform.h"
#include "comp_player_input.h"
#include "components/fsm/comp_fsm.h"

DECL_OBJ_MANAGER("player_input", TCompPlayerInput);

void TCompPlayerInput::debugInMenu() {
}

void TCompPlayerInput::load(const json& j, TEntityParseContext& ctx) {
}

void TCompPlayerInput::update(float dt)
{
  TMsgSetFSMVariable jumpMsg;
  jumpMsg.variant.setName("jump");
  jumpMsg.variant.setBool(EngineInput["jump"].isPressed());
  if (EngineInput["jump"].hasChanged())
  {
    CEntity* e = CHandle(this).getOwner();
    e->sendMsg(jumpMsg);
  }

  TMsgSetFSMVariable speedMsg;
  speedMsg.variant.setName("speed");
  speedMsg.variant.setFloat(EngineInput["move"].value);
  if (EngineInput["move"].hasChanged())
  {
    CEntity* e = CHandle(this).getOwner();
    e->sendMsg(speedMsg);
  }
}
