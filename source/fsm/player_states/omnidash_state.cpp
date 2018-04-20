#include "mcv_platform.h"
#include "omnidash_state.h"
#include "components/player/comp_player_controller.h"
#include "entity/entity_parser.h"
#include "fsm/context.h"

namespace FSM
{
	void OmnidashState::onStart(CContext& ctx) const
	{
		ctx.setVariable("can_omni", false);
		CEntity* e = ctx.getOwner();
		TCompPlayerController* player = e->get<TCompPlayerController>();
		player->change_mesh(player->EAnimations::EOmni);
		EngineTimer.setTimeSlower(0.25f);
	}

	bool OmnidashState::load(const json& jData)
	{
		return true;
	}

	bool OmnidashState::update(float dt, CContext& ctx) const
	{
		CEntity* e = ctx.getOwner();
		TCompPlayerController* player = e->get<TCompPlayerController>();

		// Chequea si hay que realizar el salto
		if (!EngineInput["omnidash"].isPressed()) {
			ctx.setVariable("omnijump", true);
		}
		
		return false;
	}

	void OmnidashState::onFinish(CContext& ctx) const {
		ctx.setVariable("omnidash", false);
		CEntity* e = ctx.getOwner();
		TCompPlayerController* player = e->get<TCompPlayerController>();
		player->y_speed_factor = 0;
		EngineTimer.setTimeSlower(1.f);

		CEntity* e_camera = EngineCameras.getActiveCamera();
		TCompCamera* c_camera = e_camera->get< TCompCamera >();
		TCompTransform *c_my_transform = e->get<TCompTransform>();
		const Input::TInterface_Mouse& mouse = EngineInput.mouse();
		VEC3 my_pos = c_my_transform->getPosition();
		VEC3 player_position;
		c_camera->getScreenCoordsOfWorldCoord(my_pos, &player_position);
		player->omnidash_arrow = mouse._position - VEC2(player_position.x, player_position.y);
		player->omnidash_arrow.Normalize();
		player->y_speed_factor = 0;

		TEntityParseContext ctx1;
		ctx1.entity_starting_the_parse = e;
		ctx1.root_transform = *(TCompTransform*)e->get<TCompTransform>();
		if (parseScene("data/prefabs/windstrike.prefab", ctx1)) {
		assert(!ctx1.entities_loaded.empty());
		TMsgChangeDirection MsgChangeDirection;
		VEC3 omni_vector = c_my_transform->getFront();
		if (player->looking_left)
		omni_vector *= player->omnidash_arrow.x;
		else
		omni_vector *= player->omnidash_arrow.x * -1;

		omni_vector.y += player->omnidash_arrow.y;
		MsgChangeDirection.new_direction = -omni_vector;
		CEntity *e = ctx1.current_entity;
		e->sendMsg(MsgChangeDirection);
		}
	}
}