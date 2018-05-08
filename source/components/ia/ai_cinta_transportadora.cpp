#include "mcv_platform.h"
#include "handle/handle.h"
#include "ai_cinta_transportadora.h"
#include "entity/entity_parser.h"
#include "components/juan/comp_transform.h"
#include "render/render_utils.h"
#include "modules/system/module_physics.h"
#include "components/player/comp_player_controller.h"

using namespace physx;

DECL_OBJ_MANAGER("ai_cinta_transportadora", CAICintaTransportadora);

void CAICintaTransportadora::registerMsgs() {
	DECL_MSG(CAICintaTransportadora, TMsgAttachTo, attachPlayer);
	DECL_MSG(CAICintaTransportadora, TMsgDetachOf, detachPlayer);
}

void CAICintaTransportadora::attachPlayer(const TMsgAttachTo& msg) {
	attached = msg.h_attacher;
}

void CAICintaTransportadora::detachPlayer(const TMsgDetachOf& msg) {
	attached = CHandle();
}

void CAICintaTransportadora::debugInMenu() {

	IAIController::debugInMenu();
}

void CAICintaTransportadora::load(const json& j, TEntityParseContext& ctx) {
	setEntity(ctx.current_entity);

	auto& j_configs = j["configs"];
	for (auto it = j_configs.begin(); it != j_configs.end(); ++it) {
		TConfig conf;
		conf.state_speed = it.value()["speed"];
		conf.state_left = it.value()["left"];
		conf.state_time = it.value()["time"];
		config_states.push_back(conf);
	}

	Init();
}

void CAICintaTransportadora::Init()
{
	// insert all states in the map
	AddState("moving_state", (statehandler)&CAICintaTransportadora::MovingState);
	AddState("next_config_state", (statehandler)&CAICintaTransportadora::NextConfigState);


	// reset the state
	//load_config();
	it_config = 0;
	ChangeState("moving_state");
}

void CAICintaTransportadora::load_config() {
	//Provarem de fer-ho sense necessitat de carregar la config en unes variables globals i accedir directament al vector
};

void CAICintaTransportadora::NextConfigState() {
	++it_config;
	if (it_config >= config_states.size()) it_config = 0;
	current_time = 0.f;
	ChangeState("moving_state");
};

void CAICintaTransportadora::MovingState(float dt) {
	if (attached.isValid()) {
		CEntity* e = attached;
		assert(e);
		TCompCollider *player_collider = e->get< TCompCollider >();
		TCompTransform *player_transform = e->get< TCompTransform >();
		TCompPlayerController *player_controller = e->get<TCompPlayerController>();

		VEC3 p_position = player_transform->getPosition();
		float p_y, p_p;
		player_transform->getYawPitchRoll(&p_y, &p_p);
		
		bool move_right = player_transform->isInLeft(player_controller->center);
		if (config_states[it_config].state_left)
		{
			p_y -= DT * config_states[it_config].state_speed;
		}
		else
		{
			p_y += DT * config_states[it_config].state_speed;
		}
		player_transform->setYawPitchRoll(p_y, p_p);
		VEC3 center = player_controller->center;
		center.y = p_position.y;
		VEC3 aux_vector = move_right ? -1 * player_transform->getLeft() : player_transform->getLeft();
		VEC3 newPos = center + (aux_vector * player_controller->tower_radius);
		VEC3 delta_pos = newPos - p_position;
		player_collider->controller->move(physx::PxVec3(delta_pos.x, delta_pos.y, delta_pos.z), 0.f, DT, physx::PxControllerFilters());
	}
	current_time += DT;
	if (current_time > config_states[it_config].state_time)
		ChangeState("next_config_state");
};




