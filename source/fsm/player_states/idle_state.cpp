#include "mcv_platform.h"
#include "idle_state.h"
#include "fsm/context.h"
#include "components/player/comp_player_controller.h"
#include "components/fsm/comp_fsm.h"
#include "components/physics/controller_filter.h"
#include "components/physics/query_filter.h"


namespace FSM
{
	void IdleState::onStart(CContext& ctx) const
	{
		CEntity* e = ctx.getOwner();
		TCompPlayerController* player = e->get<TCompPlayerController>();
 		// Pongo la animacion solo si no se esta moviendo
		ctx.setVariable("run", false);
		player->change_animation(player->EAnimations::NajaIdle, _is_action, _delay_in, _delay_out, true);
		ctx.setVariable("initial", false);
        player->idle_time = 0;
	}

	bool IdleState::load(const json& jData)
	{
		_is_action = jData.value("is_action", false);
		_delay_out = jData.value("delay_out", 0.01f);
		_delay_in = jData.value("delay_in", 0.01f);
		return true;
	}

	bool IdleState::update(float dt, CContext& ctx) const
	{
		CEntity* e = ctx.getOwner();
		TCompPlayerController* player = e->get<TCompPlayerController>();
        player->idle_time += dt;
		TCompCollider* comp_collider = e->get<TCompCollider>();
		TCompTransform *c_my_transform = e->get<TCompTransform>();
		float y_speed = (player->y_speed_factor * dt) - (player->gravity * dt * dt / 2);
		if (!player->is_grounded)
			player->y_speed_factor -= player->gravity * dt / 2;

		
		if (comp_collider && comp_collider->controller)
		{

			PxShape* player_shape;
			comp_collider->controller->getActor()->getShapes(&player_shape, 1);
			PxFilterData filter_data = player_shape->getSimulationFilterData();
			ControllerFilterCallback *filter_controller = new ControllerFilterCallback();
			BasicQueryFilterCallback *query_filter = new BasicQueryFilterCallback();
			PxControllerCollisionFlags flags = comp_collider->controller->move(PxVec3(0, y_speed, 0), 0.f, dt, PxControllerFilters(&filter_data, query_filter, filter_controller));

			if (flags.isSet(physx::PxControllerCollisionFlag::eCOLLISION_DOWN) && !player->is_grounded) {
				if (player->jumping_start_height - c_my_transform->getPosition().y > player->jumping_death_height) {
					ctx.setVariable("hit", true);
				}
				player->is_grounded = true;
				ctx.setVariable("is_grounded", true);
				ctx.setVariable("can_omni", true);
				ctx.setVariable("can_dash", true);
			}
			else if (!flags.isSet(physx::PxControllerCollisionFlag::eCOLLISION_DOWN) && player->is_grounded) {
				player->is_grounded = false;
				ctx.setVariable("is_grounded", false);
				player->jumping_start_height = c_my_transform->getPosition().y;
				ctx.setVariable("is_falling", true);
			}
			else if (!flags.isSet(physx::PxControllerCollisionFlag::eCOLLISION_DOWN)) {
 				ctx.setVariable("is_falling", true);
			}
		}

        if (player->idle_time > player->idle_max_time) {
            CEntity* camera_manager = (CEntity*)getEntityByName("camera_manager");
            TMsgActiveCamera activate_camera;
            activate_camera.camera_name = "camera_idle";
            activate_camera.blend_time = 4.f;
            camera_manager->sendMsg(activate_camera);
        }
		return false;
	}

	void IdleState::onFinish(CContext& ctx) const {
		ctx.setVariable("idle", false);
        CEntity* e = ctx.getOwner();
        TCompPlayerController* player = e->get<TCompPlayerController>();
        if (player->idle_time > player->idle_max_time) {
            CEntity* camera_manager = (CEntity*)getEntityByName("camera_manager");
            TMsgActiveCamera activate_camera;
            activate_camera.camera_name = "camera_orbit_IZQ";
            activate_camera.blend_time = 2.f;
            camera_manager->sendMsg(activate_camera);
        }
	}
}