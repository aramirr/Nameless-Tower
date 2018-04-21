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
	CEntity* e = CHandle(this).getOwner();

  TMsgSetFSMVariable jumpMsg;
  jumpMsg.variant.setName("jump");
  jumpMsg.variant.setBool(EngineInput["jump"].isPressed());
  if (EngineInput["jump"].hasChanged())
  {
    
    e->sendMsg(jumpMsg);
  }

	TMsgSetFSMVariable dashMsg;
	dashMsg.variant.setName("dash");
	dashMsg.variant.setBool(EngineInput["dash"].isPressed());
	if (EngineInput["dash"].hasChanged())
	{
		e->sendMsg(dashMsg);
	}

	TMsgSetFSMVariable windstrikeMsg;
	windstrikeMsg.variant.setName("windstrike");
	windstrikeMsg.variant.setBool(EngineInput["windstrike"].isPressed());
	if (EngineInput["windstrike"].hasChanged())
	{
		e->sendMsg(windstrikeMsg);
	}

	TMsgSetFSMVariable omnidashMsg;
	omnidashMsg.variant.setName("omnidash");
	omnidashMsg.variant.setBool(EngineInput["omnidash"].isPressed());
	if (EngineInput["omnidash"].hasChanged())
	{
		e->sendMsg(omnidashMsg);
	}

	TMsgSetFSMVariable runMsg;
	runMsg.variant.setName("run");
	runMsg.variant.setBool(EngineInput["left"].isPressed());
	if (EngineInput["left"].isPressed())
	{
		e->sendMsg(runMsg);
	}
	runMsg.variant.setBool(EngineInput["right"].isPressed());
	if (EngineInput["right"].isPressed())
	{
		e->sendMsg(runMsg);
	}
	

	TMsgSetFSMVariable respawnMsg;
	respawnMsg.variant.setName("initial");
	respawnMsg.variant.setBool(true);
	if (EngineInput["respawn_player"].hasChanged())
	{
		e->sendMsg(respawnMsg);
	}

	TMsgSetFSMVariable deadMsg;
	deadMsg.variant.setName("hit");
	deadMsg.variant.setBool(true);
	if (EngineInput["kill"].hasChanged())
	{
		e->sendMsg(deadMsg);
	}

	TMsgSetFSMVariable glideMsg;
	glideMsg.variant.setName("glide");
	glideMsg.variant.setBool(EngineInput["glide"].isPressed());
	if (EngineInput["glide"].hasChanged())
	{
		e->sendMsg(glideMsg);
	}
	/*
	TMsgSetFSMVariable pauseMsg;
	pauseMsg.variant.setName("pause");
	pauseMsg.variant.setBool(EngineInput["pause"].getsPressed());
	if (EngineInput["pause"].hasChanged())
	{
		CEntity* e = CHandle(this).getOwner();
		e->sendMsg(pauseMsg);
	}
	*/
}
