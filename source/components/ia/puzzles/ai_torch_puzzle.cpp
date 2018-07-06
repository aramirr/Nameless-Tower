#include "mcv_platform.h"
#include "handle/handle.h"
#include "ai_torch_puzzle.h"
#include "entity/entity_parser.h"
#include "components/juan/comp_transform.h"
#include "render/render_utils.h"
#include "modules/system/module_physics.h"
#include "components/player/comp_player_controller.h"


DECL_OBJ_MANAGER("ai_torch_puzzle", CAITorchPuzzle);

void CAITorchPuzzle::Init()
{
	AddState("active", (statehandler)&CAITorchPuzzle::ActiveState);
    AddState("inactive", (statehandler)&CAITorchPuzzle::InactiveState);
    AddState("complete", (statehandler)&CAITorchPuzzle::CompleteState);
	active = false;
	ChangeState("inactive");
}

void CAITorchPuzzle::debugInMenu() {

	IAIController::debugInMenu();
}

void CAITorchPuzzle::load(const json& j, TEntityParseContext& ctx) {
    setEntity(ctx.current_entity);
    timer_limit = j.value("time_limit", 5.0f);
    door_name = j.value("door", "");
	Init();
}

void CAITorchPuzzle::registerMsgs() {
    DECL_MSG(CAITorchPuzzle, TMsgActivateTorchPuzzle, activate);
    DECL_MSG(CAITorchPuzzle, TMsgAttachTo, attach);
}


void CAITorchPuzzle::ActiveState(float dt)
{	
	timer += dt;
	if (timer > timer_limit) {
		deactivate();
	}
}


void CAITorchPuzzle::CompleteState(float dt)
{
    CEntity* entity = (CEntity*)getEntityByName(door_name);
    TMsgOpenDoor msg;
    entity->sendMsg(msg);
}


void CAITorchPuzzle::InactiveState(float dt)
{		
}

void CAITorchPuzzle::deactivate() {
	active = false;
	TCompRender *my_render = getMyRender();
	my_render->self_illumination = 1;
	ChangeState("inactive");
    for (auto torch : torchs) {
		CAITorch* t = torch->get<CAITorch>();
        t->activate();
    }
    activated_torchs = 0;
}


void CAITorchPuzzle::activate(const TMsgActivateTorchPuzzle& msg) {
    activated_torchs++;
	if (!active) {
		active = true;
		timer = 0;
		ChangeState("active");        
	}	
    if (activated_torchs == torchs.size())
        ChangeState("complete");
}

void CAITorchPuzzle::attach(const TMsgAttachTo& msg) {
    CEntity* e = msg.h_attacher;
    torchs.push_back(e);
}
