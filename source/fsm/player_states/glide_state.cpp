#include "mcv_platform.h"
#include "glide_state.h"
#include "fsm/context.h"
#include "components/player/comp_player_controller.h"
#include "modules/system/module_physics.h"
#include "components/physics/controller_filter.h"
#include "components/physics/query_filter.h"

using namespace physx;

namespace FSM
{
	void GlideState::onStart(CContext& ctx) const
	{
		CEntity* e = ctx.getOwner();
		TCompPlayerController* player = e->get<TCompPlayerController>();
		_sound->setVolume(player->volumen);
		_sound_cloth_rustle->setVolume(player->volumen);
		TCompTransform *c_my_transform = e->get<TCompTransform>();
		ctx.setVariable("is_grounded", false);
		player->is_grounded = false;
		player->change_animation(player->EAnimations::NajaGlideStrike, true, _delay_in, _delay_out, true);
		player->change_animation(player->EAnimations::NajaGlideLoop, _is_action, 0.1f, _delay_out, true);
        _sound_cloth_rustle->start();
	}

	bool GlideState::load(const json& jData)
	{
		_x_speed = jData.value("x_speed", 2.f);
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
            event_description_cloth->createInstance(&_sound_cloth_rustle);
        }
		return true;
	}

	bool GlideState::update(float dt, CContext& ctx) const
	{		
		CEntity* e = ctx.getOwner();
		TCompPlayerController* player = e->get<TCompPlayerController>();
		TCompTransform *c_my_transform = e->get<TCompTransform>();
		
		TCompCollider *e_col = e->get<TCompCollider>();
	
		//Mitigate force by distance from Center of the fan
		//float distance_from_transform = VEC3::Distance(e_transform->getPosition(), my_transform->getPosition());
		VEC3 direction = VEC3(0.f,6.f,0.f) * dt;
		

		PxRigidActor* rigidActor = ((PxRigidActor*)e_col->actor);
		PxTransform tr = rigidActor->getGlobalPose();
		c_my_transform->setPosition(VEC3(tr.p.x, tr.p.y, tr.p.z));


		VEC3 my_pos = c_my_transform->getPosition();
		VEC3 new_pos = my_pos;
		float y_speed;

		player->jumping_start_height = c_my_transform->getPosition().y;
		// Chequea movimiento
		if (EngineInput["left"].isPressed()) {
			if (!player->looking_left) {
				player->looking_left = true;
				player->move_player(false, true, dt, 0, 2);
			}
			else {
				player->move_player(false, false, dt, 0, 2);
			}
		}
		else if (EngineInput["right"].isPressed()) {
			if (!player->looking_left) {
				player->move_player(true, false, dt, 0, 2);
			}
			else {
				player->looking_left = false;
				player->move_player(true, true, dt, 0, 2);
			}
		}


		PxShape* player_shape;
		e_col->controller->getActor()->getShapes(&player_shape, 1);
		PxFilterData filter_data = player_shape->getSimulationFilterData();
		ControllerFilterCallback *filter_controller = new ControllerFilterCallback();
		BasicQueryFilterCallback *query_filter = new BasicQueryFilterCallback();
		e_col->controller->move(PxVec3(direction.x, direction.y, direction.z), 0.f, dt, PxControllerFilters(&filter_data, query_filter, filter_controller));

		return false;
	}

	void GlideState::onFinish(CContext& ctx) const {
		ctx.setVariable("glide", false);
		CEntity* e = ctx.getOwner();
		TCompPlayerController* player = e->get<TCompPlayerController>();
		player->y_speed_factor = 0;
        player->previous_state = "glide";
	}

}

