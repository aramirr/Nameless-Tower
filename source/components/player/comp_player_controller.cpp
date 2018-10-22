#include "mcv_platform.h"
#include "entity/entity_parser.h"
#include "comp_player_controller.h"
#include "components/juan/comp_transform.h"
#include "components/camera/comp_camera.h"
#include "components/ui/ui_mouse_pos.h"
#include "entity/common_msgs.h"
#include "components/fsm/comp_fsm.h"
#include "components/physics/controller_filter.h"
#include "components/physics/query_filter.h"
#include "skeleton/comp_skeleton.h"
#include "components/comp_particles.h"

DECL_OBJ_MANAGER("player_controller", TCompPlayerController);

void TCompPlayerController::debugInMenu() {
	ImGui::DragFloat("Y speed: %f", &y_speed_factor, 0.01f, 0.f, 100.f);
    ImGui::DragFloat("Gravity: %f", &gravity, 0.01f, 0.f, 200.f);
    ImGui::Text("Can die: %s", can_die ? "Si" : "No");
		ImGui::Text("Cinematic: %s", on_cinematic ? "Si" : "No");
		ImGui::Text("Grounded: %s", is_grounded ? "Si" : "No");
}

void TCompPlayerController::load(const json& j, TEntityParseContext& ctx) {
	setEntity(ctx.current_entity);
	gravity = j.value("gravity", 75.f);
	center = VEC3(0.f, 0.f, 0.f);
	tower_radius = j.value("tower_radius", 31.5f);
	jumping_death_height = j.value("jumping_death_height", 9.f);
	idle_max_time = j.value("idle_max_time", 10.f);
	is_grounded = true;
	looking_left = false;
	level_started = false;
    rayWall = false;
    on_cinematic = false;
    Studio::EventDescription* event_description = NULL;
    std::string event_name = "event:/SFX/Character/Naja/StepsLands/Land_Dirt";
    EngineSound.system->getEvent(event_name.c_str(), &event_description);
    event_description->createInstance(&_sound_land);

	init();
}

void TCompPlayerController::init() {}

TCompTransform* TCompPlayerController::getMyTransform() {
	TCompTransform* c = h_transform;
	if (!c) {
		CEntity* e = h_entity;
		assert(e);
		h_transform = e->get< TCompTransform >();
		assert(h_transform.isValid());
		c = h_transform;
	}
	return c;
}

TCompRender* TCompPlayerController::getMyRender() {
	TCompRender* r = h_render;
	if (!r) {
		CEntity* e = h_entity;
		assert(e);
		h_render = e->get< TCompRender >();
		assert(h_render.isValid());
		r = h_render;
	}
	return r;
}

TCompCollider* TCompPlayerController::getMyCollider() {
	TCompCollider* c = h_collider;
	if (!c) {
		CEntity* e = h_entity;
		assert(e);
		h_collider = e->get< TCompCollider >();
		assert(h_collider.isValid());
		c = h_collider;
	}
	return c;
}

void TCompPlayerController::setEntity(CHandle new_entity) {
	h_entity = new_entity;
	assert(h_entity.isValid());
}

void TCompPlayerController::change_animation(int animation_id, bool is_action, float in_delay, float out_delay, bool clear = true) {
	CEntity* e = h_entity;
	TCompSkeleton* skeleton = e->get<TCompSkeleton>();
	assert(skeleton);

	skeleton->playAnimation(animation_id, is_action, in_delay, out_delay, clear);
}

void TCompPlayerController::remove_animation(int animation_id, float delay) {
	CEntity* e = h_entity;
	TCompSkeleton* skeleton = e->get<TCompSkeleton>();
	assert(skeleton);

	skeleton->removeAnimation(animation_id, delay);
}

void TCompPlayerController::clear_animations(float out_delay) {
	CEntity* e = h_entity;
	TCompSkeleton* skeleton = e->get<TCompSkeleton>();
	assert(skeleton);

	skeleton->clearActions(out_delay);
}

void TCompPlayerController::move_player(bool left, bool change_orientation, float dt, float y_speed, float x_speed) {
	TCompTransform *c_my_transform = get<TCompTransform>();

	assert(c_my_transform);
	// Current orientation
	float current_yaw;
	float current_pitch;
	float amount_moved = x_speed * dt;
	c_my_transform->getYawPitchRoll(&current_yaw, &current_pitch);

	VEC3 myPos = c_my_transform->getPosition();
	center.y = myPos.y;
	//float distance = VEC3::Distance(myPos, center);
	VEC3 move_vector = center + myPos;

	if (change_orientation) {
		current_yaw = left ? current_yaw + deg2rad(180) : current_yaw - deg2rad(180);
		c_my_transform->setYawPitchRoll(current_yaw, current_pitch);
		TMsgSetFSMVariable turnMsg;
		turnMsg.variant.setName("turn_around");
		turnMsg.variant.setBool(true);
		CEntity* e = CHandle(this).getOwner();
		e->sendMsg(turnMsg);
		CEntity* particles_emiter;
	}
	else {
		current_yaw = left ? current_yaw + 0.1f * amount_moved : current_yaw - 0.1f * amount_moved;
		c_my_transform->setYawPitchRoll(current_yaw, current_pitch);
		VEC3 aux_vector = left ? -1 * c_my_transform->getLeft() : c_my_transform->getLeft();
		VEC3 newPos = center + (aux_vector * tower_radius);
		TCompCollider* comp_collider = get<TCompCollider>();
		if (comp_collider && comp_collider->controller)
		{
			VEC3 delta_move = newPos - myPos;
			delta_move.y += y_speed;
			PxShape* player_shape;
			comp_collider->controller->getActor()->getShapes(&player_shape, 1);
			PxFilterData filter_data = player_shape->getSimulationFilterData();
			ControllerFilterCallback *filter_controller = new ControllerFilterCallback();
			BasicQueryFilterCallback *query_filter = new BasicQueryFilterCallback();

			//Raycast looking for walls
			VEC3 player_position = c_my_transform->getPosition();
			VEC3 player_front = c_my_transform->getFront();

			//FRONT
			VEC3 posef = player_position + player_front * 0.3f;
			PxVec3 originf = PxVec3(posef.x, posef.y + 0.5f, posef.z);
			PxVec3 unitDirf = PxVec3(player_front.x, player_front.y, player_front.z);

			//BACK
			VEC3 poseb = player_position - player_front * 0.3f;
			PxVec3 originb = PxVec3(poseb.x, poseb.y + 0.5f, poseb.z);
			PxVec3 unitDirb = PxVec3(-player_front.x, -player_front.y, -player_front.z);

			PxReal maxDistance = 4.0f;
			PxRaycastBuffer hit;

			if ((EnginePhysics.gScene->raycast(originf, unitDirf, maxDistance, hit)) || (EnginePhysics.gScene->raycast(originb, unitDirb, maxDistance / 2, hit))) {
				PxFilterData filter_data = hit.block.shape->getSimulationFilterData();

				if (filter_data.word0 == CModulePhysics::FilterGroup::Scenario) {
					//TODO: Mandar mensaje a la camara para que se quede quieta -> MANUE el LLoron
					CEntity* e = (CEntity*)getEntityByName("camera_orbit_IZQ");
					rayWall = true;
					TMsgDeactiveCamera msg;
					e->sendMsg(msg);
					e = (CEntity*)getEntityByName("camera_look_up");
					e->sendMsg(msg);
					e = (CEntity*)getEntityByName("camera_look_down");
					e->sendMsg(msg);
				}
				else {
					CEntity* e = (CEntity*)getEntityByName("camera_orbit_IZQ");
					rayWall = false;
					TMsgActiveCamera msg;
					msg.blend_time = 2.f;
					e->sendMsg(msg);
					e = (CEntity*)getEntityByName("camera_look_up");
					e->sendMsg(msg);
					e = (CEntity*)getEntityByName("camera_look_down");
					e->sendMsg(msg);
				}

			}
			else  {
				//if (rayWall && filter_data.word0 != CModulePhysics::FilterGroup::Scenario) {
					CEntity* e = (CEntity*)getEntityByName("camera_orbit_IZQ");
					rayWall = false;
					TMsgActiveCamera msg;
					msg.blend_time = 2.f;
					e->sendMsg(msg);
					e = (CEntity*)getEntityByName("camera_look_up");
					e->sendMsg(msg);
					e = (CEntity*)getEntityByName("camera_look_down");
					e->sendMsg(msg);
				//}
				
			}

			PxControllerCollisionFlags flags = comp_collider->controller->move(PxVec3(delta_move.x, delta_move.y, delta_move.z), 0.f, dt, PxControllerFilters(&filter_data, query_filter, filter_controller));

			if (flags.isSet(physx::PxControllerCollisionFlag::eCOLLISION_DOWN) && !is_grounded) {
				if (jumping_start_height - c_my_transform->getPosition().y > jumping_death_height) {
					TMsgSetFSMVariable deadMsg;
					deadMsg.variant.setName("hit");
					deadMsg.variant.setBool(true);
					CEntity* e = CHandle(this).getOwner();
					e->sendMsg(deadMsg);

					//dbg("NAPOLEO\n");
					CEntity* runner = (CEntity *)getEntityByName("Runner");
					TMsgRunnerDisappear msg_disappear;
					runner->sendMsg(msg_disappear);
				}
                CEntity* particles_emiter = (CEntity*)getEntityByName("humo_land");
                TCompParticles* c_particles = particles_emiter->get<TCompParticles>();
                c_particles->emit();
				is_grounded = true;				
                _sound_land->start();
                change_animation(EAnimations::NajaJumpLand, true, 0.01, 0.1, false);
				TMsgSetFSMVariable groundMsg;
				groundMsg.variant.setName("is_grounded");
				groundMsg.variant.setBool(true);
				CEntity* e = CHandle(this).getOwner();
				e->sendMsg(groundMsg);
				TMsgSetFSMVariable dashMsg;
				dashMsg.variant.setName("can_dash");
				dashMsg.variant.setBool(true);
				e->sendMsg(dashMsg);
				TMsgSetFSMVariable omniMsg;
				omniMsg.variant.setName("can_omni");
				omniMsg.variant.setBool(true);
				e->sendMsg(omniMsg);
			}
			else if (!flags.isSet(physx::PxControllerCollisionFlag::eCOLLISION_DOWN) && is_grounded) {
				//FRONT
				VEC3 posef = player_position;
				PxVec3 originf = PxVec3(posef.x, posef.y, posef.z);
				PxVec3 unitDirf = PxVec3(0, -1, 0);

				PxReal maxDistance = 0.5f;
				PxRaycastBuffer hit;
				TMsgSetFSMVariable dashMsg;
				dashMsg.variant.setName("can_dash");
				dashMsg.variant.setBool(true);
				CEntity* e = CHandle(this).getOwner();
				e->sendMsg(dashMsg);
				if (!EnginePhysics.gScene->raycast(originf, unitDirf, maxDistance, hit)) {
					is_grounded = false;
					y_speed_factor = 0;
					jumping_start_height = c_my_transform->getPosition().y;
					TMsgSetFSMVariable groundMsg;
					groundMsg.variant.setName("is_grounded");
					groundMsg.variant.setBool(false);
					CEntity* e = CHandle(this).getOwner();
					e->sendMsg(groundMsg);
					TMsgSetFSMVariable fallMsg;
					fallMsg.variant.setName("is_falling");
					fallMsg.variant.setBool(true);
					e->sendMsg(fallMsg);
				}
			}

			if (flags.isSet(physx::PxControllerCollisionFlag::eCOLLISION_SIDES)) {
				current_yaw = left ? current_yaw - 0.1f * amount_moved : current_yaw + 0.1f * amount_moved;
				c_my_transform->setYawPitchRoll(current_yaw, current_pitch);
			}
			else if (flags.isSet(physx::PxControllerCollisionFlag::eCOLLISION_UP)) {
				/*TMsgSetFSMVariable idleMsg;
				idleMsg.variant.setName("idle");
				idleMsg.variant.setBool(true);
				CEntity* e = CHandle(this).getOwner();
				e->sendMsg(idleMsg);*/
			}
		}
		else
		{
			//Actualizo la posicion del transform
			c_my_transform->setPosition(newPos);
		}
	}
}

void TCompPlayerController::setCheckpoint(const TMsgCheckpoint& msg)
{
	//dbg("Checkpoint\n");
	checkpoint = msg.appearing_position;
}
