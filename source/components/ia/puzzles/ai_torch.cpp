#include "mcv_platform.h"
#include "handle/handle.h"
#include "ai_torch.h"
#include "entity/entity_parser.h"
#include "components/juan/comp_transform.h"
#include "render/render_utils.h"
#include "modules/system/module_physics.h"
#include "components/player/comp_player_controller.h"


DECL_OBJ_MANAGER("ai_torch", CAITorch);

void CAITorch::Init()
{
	AddState("active", (statehandler)&CAITorch::ActiveState);
	AddState("inactive", (statehandler)&CAITorch::InactiveState);
	ChangeState("active");
}

void CAITorch::debugInMenu() {

	IAIController::debugInMenu();
}

void CAITorch::load(const json& j, TEntityParseContext& ctx) {
    setEntity(ctx.current_entity);
	timer_limit = j.value("time_limit", 5.0f);
	puzzle_name = j.value("puzzle", "");
	if (puzzle_name != "") {
		in_puzzle = true;
		CEntity* puzzle_entity = (CEntity*)getEntityByName(puzzle_name);
        TMsgAttachTo activate_msg;
        CEntity* e = h_entity;
        activate_msg.h_attacher = h_entity;
        puzzle_entity->sendMsg(activate_msg);
	}
	Init();
}

void CAITorch::registerMsgs() {
DECL_MSG(CAITorch, TMsgDeactivateTorch, deactivate);
}


void CAITorch::ActiveState(float dt)
{	
}


void CAITorch::InactiveState(float dt)
{	
	if (!in_puzzle) {
		timer += dt;
		if (timer > timer_limit) {
			activate();
		}
	}	
}

void CAITorch::activate() {
	active = true;
	TCompRender *my_render = getMyRender();
	my_render->self_illumination = 1;
	ChangeState("active");
}


void CAITorch::deactivate(const TMsgDeactivateTorch& msg) {
	if (active) {
		active = false;
		TCompRender *my_render = getMyRender();
		my_render->self_illumination = 1;
		timer = 0;
		ChangeState("inactive");
		if (in_puzzle) {
			TMsgActivateTorchPuzzle activate_msg;
			CEntity* e_collider_entity = (CEntity*)getEntityByName(puzzle_name);
            CEntity* e = h_entity;
			e_collider_entity->sendMsg(activate_msg);
            attached = true;
		}
	}	
}
