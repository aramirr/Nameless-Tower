#include "mcv_platform.h"
#include "dash_state.h"
#include "fsm/context.h"
#include "components/player/comp_player_controller.h"
#include "components/sound/comp_sound.h"
#include "components\comp_particles.h"

namespace FSM
{
	void DashState::onStart(CContext& ctx) const
	{
		ctx.setVariable("can_dash", false);
		CEntity* e = ctx.getOwner();
		TCompPlayerController* player = e->get<TCompPlayerController>();
		TCompTransform* c_my_transform = e->get<TCompTransform>();
		_sound->setVolume(player->volumen);
		player->jumping_start_height = c_my_transform->getPosition().y;
        player->clear_animations(0.1f);
		player->change_animation(player->EAnimations::NajaDashStrike, _is_action, _delay_in, _delay_out, true);
        _sound->start();
        CEntity* particles_emiter;
        if (player->looking_left)
            particles_emiter = (CEntity*)getEntityByName("humo_dash_left");
        else
            particles_emiter = (CEntity*)getEntityByName("humo_dash_right");
        TCompParticles* c_particles = particles_emiter->get<TCompParticles>();
        c_particles->emit();
		CEntity* trail = (CEntity *)getEntityByName("player_trail");
		if (trail)
		{
			TCompParticles* particle = (TCompParticles*)trail->get<TCompParticles>();
			particle->_core->life.maxParticles = 200;
		}
        TCompSound* sound = e->get<TCompSound>();
        sound->playSound("action");
	}

	bool DashState::load(const json& jData)
	{
		_x_speed = jData.value("x_speed", 10.f);
		_is_action = jData.value("is_action", false);
		_delay_out = jData.value("delay_out", 0.01f);
		_delay_in = jData.value("delay_in", 0.01f);
        if (jData.count("sound")) {
            Studio::EventDescription* event_description = NULL;
            std::string event_name = jData["sound"];
            EngineSound.system->getEvent(event_name.c_str(), &event_description);
            event_description->createInstance(&_sound);
        }
        if (jData.count("sound_cloth")) {
            Studio::EventDescription* event_description_cloth = NULL;
            std::string event_name_cloth = jData["sound_cloth"];
            EngineSound.system->getEvent(event_name_cloth.c_str(), &event_description_cloth);
            event_description_cloth->createInstance(&_sound);
        }
		return true;
	}

	bool DashState::update(float dt, CContext& ctx) const
	{
		CEntity* e = ctx.getOwner();
		TCompPlayerController* player = e->get<TCompPlayerController>();
		if (player->looking_left)
			player->move_player(false, false, dt, 0, _x_speed);
		else
			player->move_player(true, false, dt, 0, _x_speed);
		return false;
	}

	void DashState::onFinish(CContext& ctx) const {
		CEntity* trail = (CEntity *)getEntityByName("player_trail");
		if (trail)
		{
			TCompParticles* particle = (TCompParticles*)trail->get<TCompParticles>();
			particle->_core->life.maxParticles = 0;
		}

		ctx.setVariable("dash", false);
		CEntity* e = ctx.getOwner();
		TCompPlayerController* player = e->get<TCompPlayerController>();
        player->previous_state = "dash";
	}
}