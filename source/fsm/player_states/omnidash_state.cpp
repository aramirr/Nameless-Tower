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
		player->change_animation(player->EAnimations::NajaOmniUp, _is_action, _delay_in, _delay_out);
		TCompTransform *c_my_transform = e->get<TCompTransform>();
		player->jumping_start_height = c_my_transform->getPosition().y;
		EngineTimer.setTimeSlower(0.25f);
		EngineUI.setOmindash(true);

		_mouseStartPosition = EngineInput.mouse()._position;
	}

	bool OmnidashState::load(const json& jData)
	{
		_is_action = jData.value("is_action", false);
		_delay_out = jData.value("delay_out", 0.01f);
		_delay_in = jData.value("delay_in", 0.01f);
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
		EngineUI.setOmindash(false);

		CEntity* e_camera = EngineCameras.getActiveCamera();
		/*CEntity* render_camera = EngineCameras.getOutputCamera();
		TCompCamera* c_render_camera = render_camera->get< TCompCamera >();*/
		TCompCamera* c_camera = e_camera->get< TCompCamera >();
		TCompTransform *c_my_transform = e->get<TCompTransform>();
		const Input::TInterface_Mouse& mouse = EngineInput.mouse();
		VEC3 my_pos = c_my_transform->getPosition();
		VEC3 player_position;
		c_camera->getScreenCoordsOfWorldCoord(my_pos, &player_position);
		/*player_position.x *= c_render_camera->getViewport().width / c_camera->getViewport().width;
		player_position.y *= c_render_camera->getViewport().height / c_camera->getViewport().height;*/
		player->omnidash_arrow = mouse._position - VEC2(player_position.x /*+ 400*/, player_position.y /*+ 300*/);
		player->omnidash_arrow.Normalize();

		std::string str = std::to_string(/*player->omnidash_arrow*/player_position.x) + ", " + std::to_string(/*player->omnidash_arrow*/player_position.y) + "\n";
		dbg(str.c_str());
		std::string str2 = std::to_string(/*player->omnidash_arrow*/mouse._position.x) + ", " + std::to_string(/*player->omnidash_arrow*/mouse._position.y) + "\n";
		dbg(str2.c_str());

		player->y_speed_factor = 0;

		/*float angle = (float)(atan2(mouse._position.y - player_position.y, mouse._position.x - player_position.x));
		dbg((std::to_string(rad2deg(angle))).c_str());
		dbg("\n");
		float y, p, r;
		c_my_transform->getYawPitchRoll(&y, &p, &r);
		player->omnidash_vector = getVectorFromYawPitch(y, angle + M_PI);

		VEC2 mouseDir = player_position - EngineInput.mouse()._position;
		mouseDir.Normalize();
		player->omnidash_vector = c_my_transform->getFront() * mouseDir.x + c_my_transform->getUp() * -mouseDir.y;
		player->omnidash_vector.Normalize();*/


		TEntityParseContext ctx1;
		ctx1.entity_starting_the_parse = e;
		ctx1.root_transform = *(TCompTransform*)e->get<TCompTransform>();
		VEC3 omni_vector = c_my_transform->getFront();
		if (player->looking_left)
			omni_vector *= player->omnidash_arrow.x;
		else
			omni_vector *= player->omnidash_arrow.x * -1;

		omni_vector.y += player->omnidash_arrow.y;
		ctx1.front = -omni_vector;
		if (parseScene("data/prefabs/windstrike.prefab", ctx1)) {
			assert(!ctx1.entities_loaded.empty());			
		}
	}
}