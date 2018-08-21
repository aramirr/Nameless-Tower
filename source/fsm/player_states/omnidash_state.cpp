#include "mcv_platform.h"
#include "omnidash_state.h"
#include "components/player/comp_player_controller.h"
#include "entity/entity_parser.h"
#include "fsm/context.h"
#include "components/postfx/comp_render_blur_radial.h"

namespace FSM
{
	void OmnidashState::onStart(CContext& ctx) const
	{
		ctx.setVariable("can_omni", false);
		CEntity* e = ctx.getOwner();
		TCompPlayerController* player = e->get<TCompPlayerController>();
		player->anim1 = calculateAnimation(e);
		player->clear_animations(0.3f);
		player->change_animation(player->anim1, _is_action, _delay_in, _delay_out, true);
		TCompTransform *c_my_transform = e->get<TCompTransform>();
		player->jumping_start_height = c_my_transform->getPosition().y;
		EngineTimer.setTimeSlower(0.25f);
		EngineUI.setOmindash(true);

		//CEntity* e_camera = EngineCameras.getActiveCamera();
		CEntity* e_camera = EngineCameras.getOutputCamera();

		//////////////////////////////////////
		TCompCamera* c_camera = e_camera->get< TCompCamera >();
		VEC3 my_pos = c_my_transform->getPosition();
		VEC3 player_position;
		c_camera->getScreenCoordsOfWorldCoord(my_pos, &player_position);
		VEC2 position;
		position = VEC2(player_position.x, player_position.y);
		VEC2 screen_projected_pos;
		screen_projected_pos.x = position.x/Render.width;
		screen_projected_pos.y = position.y/Render.height;
		///////////////////////////////////////////

		TCompRenderBlurRadial* c_render_blur_radial = e_camera->get< TCompRenderBlurRadial >();
		if (c_render_blur_radial)
		{
			c_render_blur_radial->setCenter(screen_projected_pos);
			c_render_blur_radial->setActive(true);
		}

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

		///////////////////////////////////////////
		//CEntity* e_camera = EngineCameras.getActiveCamera();
		CEntity* e_camera = EngineCameras.getOutputCamera();
		TCompTransform *c_my_transform = e->get<TCompTransform>();
		TCompCamera* c_camera = e_camera->get< TCompCamera >();
		VEC3 my_pos = c_my_transform->getPosition();
		VEC3 player_position;
		c_camera->getScreenCoordsOfWorldCoord(my_pos, &player_position);
		VEC2 position;
		position = VEC2(player_position.x, player_position.y); 
		VEC2 screen_projected_pos;
		screen_projected_pos.x = position.x / Render.width;
		screen_projected_pos.y = position.y / Render.height; 
		///////////////////////////////////////////

		TCompRenderBlurRadial* c_render_blur_radial = e_camera->get< TCompRenderBlurRadial >();
		if (c_render_blur_radial)
		{
			c_render_blur_radial->setCenter(screen_projected_pos);
		}


		// Chequea si hay que realizar el salto
		if (!EngineInput["omnidash"].isPressed()) {
			ctx.setVariable("omnijump", true);
		}

		int anim = calculateAnimation(e);
		if (player->anim2 != anim) {
			if (player->anim2 != -1)
				player->remove_animation(player->anim2);
			player->anim2 = player->anim1;
			player->anim1 = anim;
			player->change_animation(player->anim1, _is_action, _delay_in, _delay_out, false);
		}
		return false;
	}

	int OmnidashState::calculateAnimation(CEntity* e) {
		CEntity* e_camera = EngineCameras.getOutputCamera();
		TCompCamera* c_camera = e_camera->get< TCompCamera >();
		TCompTransform *c_my_transform = e->get<TCompTransform>();
		const Input::TInterface_Mouse& mouse = EngineInput.mouse();
		VEC3 my_pos = c_my_transform->getPosition();
		VEC3 player_position;
		c_camera->getScreenCoordsOfWorldCoord(my_pos, &player_position);
		VEC3 omnidash_arrow = mouse._position - VEC2(player_position.x, player_position.y);
		omnidash_arrow.Normalize();
		TCompPlayerController* player = e->get<TCompPlayerController>();
		if (omnidash_arrow.x >= 0.75) {
			return player->EAnimations::NajaOmniPrepFr;
		}
		else if (omnidash_arrow.x <= -0.80){
			return player->EAnimations::NajaOmniPrepBk;
		}
		else if (omnidash_arrow.y <= -0.80) {
			return player->EAnimations::NajaOmniPrepUp;
		} 
		else if (omnidash_arrow.y >= 0.80) {
			return player->EAnimations::NajaOmniPrepDn;
		} 
		else if (omnidash_arrow.x >= 0.20 && omnidash_arrow.y >= 0.20) {
			return player->EAnimations::NajaOmniPrepFrDn;
		}
		else if (omnidash_arrow.x <= -0.20 && omnidash_arrow.y <= -0.20) {
			return player->EAnimations::NajaOmniPrepFrBk;
		}
		else if (omnidash_arrow.x <= -0.20 && omnidash_arrow.y >= 0.20) {
			return player->EAnimations::NajaOmniPrepBkDn;
		}
		else {
			return player->EAnimations::NajaOmniPrepFrUp;
		}
	}

	void OmnidashState::onFinish(CContext& ctx) const {
		ctx.setVariable("omnidash", false);
		CEntity* e = ctx.getOwner();
		TCompPlayerController* player = e->get<TCompPlayerController>();
		player->y_speed_factor = 0;
		EngineTimer.setTimeSlower(1.f);
		EngineUI.setOmindash(false);

		//CEntity* e_camera = EngineCameras.getActiveCamera();
		CEntity* e_camera = EngineCameras.getOutputCamera();

		TCompRenderBlurRadial* c_render_blur_radial = e_camera->get< TCompRenderBlurRadial >();
		if (c_render_blur_radial)
		{
			c_render_blur_radial->setActive(false);
			//c_render_blur_radial->setCenter(VEC2(0.0, 0.0));
		}
			
		

		TCompCamera* c_camera = e_camera->get< TCompCamera >();
		TCompTransform *c_my_transform = e->get<TCompTransform>();
		const Input::TInterface_Mouse& mouse = EngineInput.mouse();
		VEC3 my_pos = c_my_transform->getPosition();
		VEC3 player_position;
		c_camera->getScreenCoordsOfWorldCoord(my_pos, &player_position);

		player->omnidash_arrow = mouse._position - VEC2(player_position.x /*+ 400*/, player_position.y /*+ 300*/);
		player->omnidash_arrow.Normalize();

		std::string str = std::to_string(/*player->omnidash_arrow*/player_position.x) + ", " + std::to_string(/*player->omnidash_arrow*/player_position.y) + "\n";
		dbg(str.c_str());
		std::string str2 = std::to_string(/*player->omnidash_arrow*/mouse._position.x) + ", " + std::to_string(/*player->omnidash_arrow*/mouse._position.y) + "\n";
		dbg(str2.c_str());

		player->y_speed_factor = 0;

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
        int anim;
        if (player->omnidash_arrow.x >= 0.75) {
            anim = player->EAnimations::NajaOmniFr;
        }
        else if (player->omnidash_arrow.x <= -0.80) {
            anim = player->EAnimations::NajaOmniBk;
        }
        else if (player->omnidash_arrow.y <= -0.80) {
            anim = player->EAnimations::NajaOmniAr;
        }
        else if (player->omnidash_arrow.y >= 0.80) {
            anim = player->EAnimations::NajaOmniAb;
        }
        else if (player->omnidash_arrow.x >= 0.20 && player->omnidash_arrow.y >= 0.20) {
            anim = player->EAnimations::NajaOmniFrDn;
        }
        else if (player->omnidash_arrow.x <= -0.20 && player->omnidash_arrow.y <= -0.20) {
            anim = player->EAnimations::NajaOmniBkUp;
        }
        else if (player->omnidash_arrow.x <= -0.20 && player->omnidash_arrow.y >= 0.20) {
            anim = player->EAnimations::NajaOmniBkDn;
        }
        else {
            anim = player->EAnimations::NajaOmniFrUp;
        }
        player->change_animation(anim, true, 0, 0, true);
	}
}