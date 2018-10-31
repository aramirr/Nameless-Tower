#include "mcv_platform.h"
#include "initial_state.h"
#include "fsm/context.h"
#include "components/player/comp_player_controller.h"
#include "components\camera\comp_orbit_camera.h"

namespace FSM
{
	void InitialState::onStart(CContext& ctx) const
	{
		CEntity* e = ctx.getOwner();
		TCompPlayerController* player = e->get<TCompPlayerController>();
		_sound->setVolume(player->volumen);
		TCompTransform* my_pos = e->get<TCompTransform>();
		TCompCollider* comp_collider = e->get<TCompCollider>();
		VEC3 position;
		if (comp_collider && comp_collider->controller) {
			if (Engine.getTower().getLastCheckpoint().x) {
				VEC3 checkpoint = Engine.getTower().getLastCheckpoint();
				comp_collider->controller->setPosition(physx::PxExtendedVec3(checkpoint.x, checkpoint.y + 0.5, checkpoint.z));
				position = VEC3(checkpoint.x, checkpoint.y, checkpoint.z);

				CEntity* camera = (CEntity*)getEntityByName("camera_orbit_IZQ");
				TCompOrbitCamera* o = camera->get<TCompOrbitCamera>();
				o->setPosition(position);
				CEntity* e = getEntityByName("The Player");
				//dbg("SONY %s\n", player->game_state);
				if (player->game_state == "level_2") {
					//dbg("NINTENDO\n");
					CEntity* cinematic = (CEntity*)getEntityByName("cinematic_sala_escaleras_2");
					TMsgDeactivateCinematic deactivate_cinematic;
					cinematic->sendMsg(deactivate_cinematic);
					EngineScripting.script.doString("RespawnOnLevel2()");
				}
			}				
			else {
				player->checkpoint = my_pos->getPosition();
				position = my_pos->getPosition();
				
			}				
		}
		else {
			player->checkpoint = my_pos->getPosition();
			position = my_pos->getPosition();
            Engine.getTower().setLastCheckpoint(position);
		}

		my_pos->lookAt(position, player->center);
		float y, p, r;
		my_pos->getYawPitchRoll(&y, &p, &r);
		y -= deg2rad(90);
		my_pos->setYawPitchRoll(y, 0, 0);

		player->looking_left = my_pos->isInLeft(player->center) ? false : true;		
		player->y_speed_factor = 0;
		player->is_grounded = true;
		ctx.setVariable("can_dash", true);
		ctx.setVariable("can_omni", true);
		ctx.setVariable("is_grounded", true);
        ctx.setVariable("is_falling", false);        
	}

	bool InitialState::load(const json& jData)
	{		
		return true;
	}

	bool InitialState::update(float dt, CContext& ctx) const
	{
		return false;
	}

	void InitialState::onFinish(CContext& ctx) const {
		ctx.setVariable("initial", false);
        CEntity* e = ctx.getOwner();
        TCompPlayerController* player = e->get<TCompPlayerController>();
        player->previous_state = "initial";
	}
}

