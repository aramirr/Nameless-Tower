#include "mcv_platform.h"
#include "windstrike_state.h"
#include "fsm/context.h"
#include "entity/entity_parser.h"
#include "components/player/comp_player_controller.h"
#include "components\comp_particles.h"


namespace FSM
{
	void WindstrikeState::onStart(CContext& ctx) const
	{				
		CEntity* e = ctx.getOwner();
        TCompTransform* transform = e->get<TCompTransform>();
        TCompPlayerController* player= e->get<TCompPlayerController>();
		TEntityParseContext ctx_w;
        float current_yaw;
        float current_pitch;
        float current_roll;
        transform->getYawPitchRoll(&current_yaw, &current_pitch, &current_roll);
		ctx_w.entity_starting_the_parse = e;
		ctx_w.root_transform = *(TCompTransform*)transform;
        current_yaw = player->looking_left ? current_yaw - deg2rad(90) : current_yaw + deg2rad(90);
        current_roll = player->looking_left ? current_roll + deg2rad(180) : current_roll;
        if (player->looking_left) {
            ctx_w.root_transform.setPosition(ctx_w.root_transform.getPosition() + VEC3(0,2,0));
        }
        ctx_w.root_transform.setYawPitchRoll(current_yaw, current_pitch, current_roll);
        //ctx_w.front = ctx_w.root_transform.getFront();
		if (parseScene("data/prefabs/windstrike.prefab", ctx_w)) {
			assert(!ctx_w.entities_loaded.empty());                        
		}
		player->change_animation(player->EAnimations::NajaWindstrikeA, _is_action, _delay_in, _delay_out, true);

		/*CEntity* trail = (CEntity *)getEntityByName("windstrike_trail");
		if (trail)
		{
			TCompParticles* particle = (TCompParticles*)trail->get<TCompParticles>();
			particle->_core->life.maxParticles = 200;
		}*/
        _sound->start();
	}
	
	bool WindstrikeState::load(const json& jData)
	{		
		_is_action = jData.value("is_action", false);
		_delay_out = jData.value("delay_out", 0.01f);
		_delay_in = jData.value("delay_in", 0.01f);
        if (jData.count("sound")) {
            Studio::EventDescription* event_description = NULL;
            std::string event_name = jData["sound"];
            EngineSound.system->getEvent(event_name.c_str(), &event_description);
            event_description->createInstance(&_sound);
        }
		return true;
	}

	bool WindstrikeState::update(float dt, CContext& ctx) const
	{		
		return false;
	}

	void WindstrikeState::onFinish(CContext& ctx) const {
		/*CEntity* trail = (CEntity *)getEntityByName("windstrike_trail");
		if (trail)
		{
			TCompParticles* particle = (TCompParticles*)trail->get<TCompParticles>();
			particle->_core->life.maxParticles = 0;
		}*/

		ctx.setVariable("windstrike", false);
        CEntity* e = ctx.getOwner();
        TCompPlayerController* player = e->get<TCompPlayerController>();
        player->previous_state = "windstrike";        
	}
}

