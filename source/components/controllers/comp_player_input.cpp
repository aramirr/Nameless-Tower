#include "mcv_platform.h"
#include "comp_player_input.h"
#include "components/fsm/comp_fsm.h"

DECL_OBJ_MANAGER("player_input", TCompPlayerInput);

void TCompPlayerInput::debugInMenu() {
	ImGui::DragFloat("wind time: %f", &windstrike_time, 0.01f, 0.f, 100.f);
}

void TCompPlayerInput::load(const json& j, TEntityParseContext& ctx) {
}

void TCompPlayerInput::update(float dt)
{
	CEntity* e = CHandle(this).getOwner();
	windstrike_time += dt;
	dash_time += dt;


    TMsgSetFSMVariable glideMsg;
    glideMsg.variant.setName("glide");
    glideMsg.variant.setBool(EngineInput["glide"].isPressed());
    if (EngineInput["glide"].hasChanged())
    {
        e->sendMsg(glideMsg);
    }

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
	if (EngineInput["dash"].hasChanged() && dash_time > dash_wait_time)
	{
		if (EngineInput["dash"].isPressed())
			dash_time = 0;
		e->sendMsg(dashMsg);
	}

	TMsgSetFSMVariable windstrikeMsg;
	windstrikeMsg.variant.setName("windstrike");
	windstrikeMsg.variant.setBool(EngineInput["windstrike"].isPressed());
	if (EngineInput["windstrike"].hasChanged() && windstrike_time > windstrike_wait_time)
	{
		if (EngineInput["windstrike"].isPressed())
			windstrike_time = 0;
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

    TMsgSetFSMVariable lookUpMsg;
    lookUpMsg.variant.setName("look_up");
    lookUpMsg.variant.setBool(EngineInput["look_up"].isPressed());
    if (EngineInput["look_up"].isPressed())
    {
        e->sendMsg(lookUpMsg);
    }   

    TMsgSetFSMVariable lookDownMsg;
    lookDownMsg.variant.setName("look_down");
    lookDownMsg.variant.setBool(EngineInput["look_down"].isPressed());
    if (EngineInput["look_down"].isPressed())
    {
        e->sendMsg(lookDownMsg);
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


    if (EngineInput["level_2"].getsPressed())
    {       
        //EngineTower.setExposureAdjustment(0);
        CEngine::get().getModules().changeGameState("level_2");
    }
	
	TMsgSetFSMVariable pauseMsg;
	pauseMsg.variant.setName("pause");
	pauseMsg.variant.setBool(EngineInput["pause"].getsPressed());
	if (EngineInput["pause"].hasChanged())
	{
		CEntity* e = CHandle(this).getOwner();
		e->sendMsg(pauseMsg);
	}
	
}
