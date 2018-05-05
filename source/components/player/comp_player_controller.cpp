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

DECL_OBJ_MANAGER("player_controller", TCompPlayerController);

void TCompPlayerController::debugInMenu() {
	ImGui::DragFloat("Y speed: %f", &y_speed_factor, 0.01f, 0.f, 100.f);
	ImGui::DragFloat("Gravity: %f", &gravity, 0.01f, 0.f, 200.f);
}

void TCompPlayerController::load(const json& j, TEntityParseContext& ctx) {
	setEntity(ctx.current_entity);
	gravity = j.value("gravity", 75.f);	
	center = VEC3(0.f, 0.f, 0.f);
	tower_radius = j.value("tower_radius", 32.f);
	jumping_death_height = j.value("jumping_death_height", 9.f);
	is_grounded = true;
	looking_left = true;

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

void TCompPlayerController::change_animation(int animation_id, bool is_action, float in_delay, float out_delay) {
  CEntity* e = h_entity;
  TCompSkeleton* skeleton = e->get<TCompSkeleton>();
  assert(skeleton);
  
  skeleton->playAnimation(animation_id, is_action, in_delay, out_delay);
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
	float amount_moved = x_speed *  dt;
	c_my_transform->getYawPitchRoll(&current_yaw, &current_pitch);

	VEC3 myPos = c_my_transform->getPosition();
	center.y = myPos.y;
	//float distance = VEC3::Distance(myPos, center);
	VEC3 move_vector = center + myPos;

	if (change_orientation) {
		current_yaw = left ? current_yaw + deg2rad(180) : current_yaw - deg2rad(180);
		c_my_transform->setYawPitchRoll(current_yaw, current_pitch);
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
			PxControllerCollisionFlags flags = comp_collider->controller->move(PxVec3(delta_move.x, delta_move.y, delta_move.z), 0.f, dt, PxControllerFilters(&filter_data, query_filter, filter_controller));
			
			if (flags.isSet(physx::PxControllerCollisionFlag::eCOLLISION_DOWN) && !is_grounded) {
				if (jumping_start_height - c_my_transform->getPosition().y  > jumping_death_height) {
					TMsgSetFSMVariable deadMsg;
					deadMsg.variant.setName("hit");
					deadMsg.variant.setBool(true);
					CEntity* e = CHandle(this).getOwner();
					e->sendMsg(deadMsg);
				}
				is_grounded = true;				
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
   	checkpoint = msg.appearing_position;
}
