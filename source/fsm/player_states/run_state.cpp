#include "mcv_platform.h"
#include "run_state.h"
#include "fsm/context.h"
#include "components/player/comp_player_controller.h"

namespace FSM
{
	void RunState::onStart(CContext& ctx) const
	{		
		  
        CEntity* e = ctx.getOwner();
        TCompPlayerController* player = e->get<TCompPlayerController>();
        player->clear_animations(0.005f);
        player->run_time = 0;
		EngineSound.res = _sound->start();
	}

	bool RunState::load(const json& jData)
	{
		_x_speed = jData.value("x_speed", 2.f);
		_is_action = jData.value("is_action", false);
		_delay_out = jData.value("delay_out", 0.01f);
		_delay_in = jData.value("delay_in", 0.01f);

		if (jData.count("sound")) {
			Studio::EventDescription* event_description = NULL;
			std::string event_name = jData["sound"];
			EngineSound.res = EngineSound.system->getEvent(event_name.c_str(), &event_description);
			EngineSound.res = event_description->createInstance(&_sound);
		}		
        
		return true;
	}

	bool RunState::update(float dt, CContext& ctx) const
	{
		CEntity* e = ctx.getOwner();
		TCompPlayerController* player = e->get<TCompPlayerController>();
        player->run_time += dt;
		TCompTransform *c_my_transform = e->get<TCompTransform>();
		float y_speed = (player->y_speed_factor * dt) - (player->gravity * dt * dt / 2);
		if (!player->is_grounded)
			player->y_speed_factor -= player->gravity * dt;
		
		if (!player->is_running && (EngineInput["left"].isPressed() || EngineInput["right"].isPressed())){
            if (player->previous_state == "idle") {
                player->change_animation(player->EAnimations::NajaRun, _is_action, _delay_in, _delay_out, true);
            }
            else {
                player->change_animation(player->EAnimations::NajaRun, _is_action, _delay_in, _delay_out, true);
            }
			player->is_running = true;
		}
		if (EngineInput["left"].isPressed()) {
			if (!player->looking_left) {
				player->looking_left = true;
				player->move_player(false, true, dt, y_speed, _x_speed);
			}
			else {
				player->move_player(false, false, dt, y_speed, _x_speed);
			}
		}
		else if (EngineInput["right"].isPressed()) {
			if (!player->looking_left) {
				player->move_player(true, false, dt, y_speed, _x_speed);
			}   
			else {
				player->looking_left = false;
				player->move_player(true, true, dt, y_speed, _x_speed);
			}
		}
		else {
			// Si no sigue corriendo pasa a estado idle
			if (!EngineInput["left"].isPressed() && !EngineInput["right"].isPressed() && player->run_time > 0.1) {
				ctx.setVariable("idle", true);
			}
		}
		
		return false;
	}

	void RunState::onFinish(CContext& ctx) const {
		EngineSound.res = _sound->stop(FMOD_STUDIO_STOP_IMMEDIATE);
		ctx.setVariable("run", false);		
		CEntity* e = ctx.getOwner();
		TCompPlayerController* player = e->get<TCompPlayerController>();
		player->is_running = false;
        player->previous_state = "run";
	}
}