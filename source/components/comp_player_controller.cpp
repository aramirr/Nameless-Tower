#include "mcv_platform.h"
#include "entity/entity_parser.h"
#include "comp_player_controller.h"
#include "comp_transform.h"
#include "entity/common_msgs.h"

DECL_OBJ_MANAGER("player_controller", TCompPlayerController);

void TCompPlayerController::move_player(bool left, bool change_orientation, float dt, float gravity) {
	TCompTransform *c_my_transform = get<TCompTransform>();
	VEC3 myPos = c_my_transform->getPosition();
	assert(c_my_transform);
	// Current orientation
	float current_yaw;
	float current_pitch;
	float amount_moved = speed_factor * dt;
	c_my_transform->getYawPitchRoll(&current_yaw, &current_pitch);

	center.y = myPos.y;
	//float distance = VEC3::Distance(myPos, center);
	VEC3 move_vector = center + myPos;
	
	if (change_orientation) {
		current_yaw = left ? current_yaw + deg2rad(180) : current_yaw - deg2rad(180);
		c_my_transform->setYawPitchRoll(current_yaw, current_pitch);
	}
	else {
		current_yaw = left ? current_yaw + 0.1 * amount_moved : current_yaw - 0.1 * amount_moved;
		c_my_transform->setYawPitchRoll(current_yaw, current_pitch);
		VEC3 aux_vector = left ? -1 * c_my_transform->getLeft() : c_my_transform->getLeft();
		VEC3 newPos = center + (aux_vector * tower_radius);
		c_my_transform->setYawPitchRoll(current_yaw, current_pitch);
		TCompCollider* comp_collider = get<TCompCollider>();
		if (comp_collider && comp_collider->controller)
		{
			VEC3 delta_move = newPos - myPos;
			delta_move.y += -gravity*dt;
			physx::PxControllerCollisionFlags flags = comp_collider->controller->move(physx::PxVec3(delta_move.x, delta_move.y, delta_move.z), 0.f, dt, physx::PxControllerFilters());
			if (flags.isSet(physx::PxControllerCollisionFlag::eCOLLISION_DOWN) && !is_grounded) {
				is_grounded = true;
			}
		}
		else
		{
			//Actualizo la posicion del transform
			c_my_transform->setPosition(newPos);
		}
	}		
}

void TCompPlayerController::debugInMenu() {
	ImGui::Text("State: %s", state.c_str());
	ImGui::DragFloat("Gravity: %f", &gravity, 0.01f, 0.f, 100.f);
	ImGui::DragFloat("Jump speed: %f", &jump_speed, 0.01f, 0.f, 100.f);
}

void TCompPlayerController::load(const json& j, TEntityParseContext& ctx) {
	setEntity(ctx.current_entity);
	speed_factor = j.value("speed", 1.0f);
	dashing_max = j.value("dashing_max", 5.0f);
	gravity = j.value("gravity", 16.5f);
	jump_speed = j.value("jump_speed", 25.8f);
  center = VEC3(0.f, 0.f, 0.f);
	tower_radius = j.value("tower_radius", 15.f);
	dashing_speed = j.value("dashing_speed", 5);
	max_jump = j.value("max_jump", 5);
	omnidash_max_time = j.value("omnidash_max_time", 0.3);
	is_grounded = true;

	init();	
}

void TCompPlayerController::init() {
	// insert all states in the map
	AddState("idle", (statehandler)&TCompPlayerController::idle_state);
	AddState("initial", (statehandler)&TCompPlayerController::initial_state);
	AddState("run", (statehandler)&TCompPlayerController::running_state);
	AddState("jump", (statehandler)&TCompPlayerController::jumping_state);
	AddState("omni", (statehandler)&TCompPlayerController::omnidashing_state);
	AddState("dash", (statehandler)&TCompPlayerController::dashing_state);
	AddState("dead", (statehandler)&TCompPlayerController::dead_state);

	// reset the state
	ChangeState("initial");

}

void TCompPlayerController::initial_state(float dt) {
	TCompTransform *my_pos = getMyTransform();
	my_pos->lookAt(my_pos->getPosition(), center);
	float y, p, r;
	my_pos->getYawPitchRoll(&y, &p, &r);
	y += deg2rad(90);
	my_pos->setYawPitchRoll(y, p, r);

	looking_left = my_pos->isInLeft(center) ? false : true;
	ChangeState("idle");
}

void TCompPlayerController::idle_state(float dt) {
	TCompCollider* comp_collider = get<TCompCollider>();
	TCompTransform *c_my_transform = getMyTransform();
	if (comp_collider && comp_collider->controller)
	{
		physx::PxControllerCollisionFlags flags = comp_collider->controller->move(physx::PxVec3(0, -gravity*dt, 0), 0.f, dt, physx::PxControllerFilters());
		if (flags.isSet(physx::PxControllerCollisionFlag::eCOLLISION_DOWN) && !is_grounded) {
			is_grounded = true;
		}
	}
	// Chequea el dash
	const Input::TButton& dash = CEngine::get().getInput().host(Input::PLAYER_1).keyboard().key(VK_LSHIFT);
	if (dash.getsPressed()) {
		dashing_amount = 0;
		speed_factor = speed_factor * dashing_speed;
		change_color(VEC4(0, 1, 1, 1));
		ChangeState("dash");
	}

	float y, p, r;
	if (isPressed('A')) {
		if (!looking_left) {			
			looking_left = true;
			move_player(false, true, dt, gravity);
		}
		else {			
			move_player(false, false, dt, gravity);
		}		
		ChangeState("run");
	}
	if (isPressed('D')) {
		if (!looking_left) {
			move_player(true, false, dt, gravity);
		}
		else {
			looking_left = false;
			move_player(true, true, dt, gravity);
		}		
		ChangeState("run");
	}

	// Chequea el salto
	const Input::TButton& space = CEngine::get().getInput().host(Input::PLAYER_1).keyboard().key(VK_SPACE);
	if (space.getsPressed()) {
		change_color(VEC4(1, 0, 1, 1));
		jump_end = c_my_transform->getPosition().y + max_jump;
		is_grounded = false;
		ChangeState("jump");
	}
}

void TCompPlayerController::running_state(float dt) {
	// Compruebo si sigue corriendo

	if (isPressed('A')) {
		if (!looking_left) {
			looking_left = true;
			move_player(false, true, dt, gravity);
		}
		else {
			move_player(false, false, dt, gravity);
		}		
	} else if (isPressed('D')) {
		if (!looking_left) {
			move_player(true, false, dt, gravity);
		}
		else {
			looking_left = false;
			move_player(true, true, dt, gravity);
		}		
	}
	// Si no sigue corriendo pasa a estado idle
	if (!isPressed('A') && !isPressed('D')){
		change_color(VEC4(1, 1, 1, 1));
		ChangeState("idle");
	}

	// Chequea el salto
	const Input::TButton& space = CEngine::get().getInput().host(Input::PLAYER_1).keyboard().key(VK_SPACE);
	if (space.getsPressed() && is_grounded) {
		change_color(VEC4(1, 0, 1, 1));
		TCompTransform *c_my_transform = get<TCompTransform>();
		jump_end = c_my_transform->getPosition().y + max_jump;
		is_grounded = false;
		ChangeState("jump");
	}
	// Chequea el dash
	const Input::TButton& dash = CEngine::get().getInput().host(Input::PLAYER_1).keyboard().key(VK_LSHIFT);
	if (dash.getsPressed()) {
		dashing_amount = 0;
		speed_factor = speed_factor * dashing_speed;
		change_color(VEC4(0, 1, 1, 1));
		ChangeState("dash");
	}
}

void TCompPlayerController::jumping_state(float dt) {
	
	TCompCollider* comp_collider = get<TCompCollider>();
	TCompTransform *c_my_transform = get<TCompTransform>();
	assert(c_my_transform);
	VEC3 my_pos = c_my_transform->getPosition();
	VEC3 new_pos = my_pos;
	new_pos.y += (jump_speed - gravity * dt) * dt;
	if (new_pos.y < jump_end) {
		if (isPressed('A')) {
			if (!looking_left) {
				looking_left = true;
				move_player(false, true, dt, -(jump_speed - gravity * dt));
			}
			else {
				move_player(false, false, dt, -(jump_speed - gravity * dt));
			}
		}
		else if (isPressed('D')) {
			if (!looking_left) {
				move_player(true, false, dt, -(jump_speed - gravity * dt));
			}
			else {
				looking_left = false;
				move_player(true, true, dt, -(jump_speed - gravity * dt));
			}
		}	else {
			VEC3 delta_move = new_pos - my_pos;
			comp_collider->controller->move(physx::PxVec3(delta_move.x, delta_move.y, delta_move.z), 0.f, dt, physx::PxControllerFilters());
		}

		// Chequea si se presiona el omnidash
		const Input::TButton& omni = CEngine::get().getInput().host(Input::PLAYER_1).mouse().button(Input::MOUSE_LEFT);
		if (omni.getsPressed()) {
			change_color(VEC4(0, 1, 0, 1));
			EngineTimer.setTimeSlower(0.25f);
			ChangeState("omni");
		}
	}
	else {
		change_color(VEC4(1, 1, 1, 1));
		ChangeState("idle");
	}
}

void TCompPlayerController::omnidashing_state(float dt) {
	omnidash_timer += dt;
	//const VEC2 _position = CEngine::get().getInput().host(Input::PLAYER_1).mouse()._position;
	if (omnidash_timer < omnidash_max_time) {
		const Input::TButton& omni = CEngine::get().getInput().host(Input::PLAYER_1).mouse().button(Input::MOUSE_LEFT);
		if (!omni.isPressed()) {
			EngineTimer.setTimeSlower(1.f);
			change_color(VEC4(1, 1, 1, 1));
			omnidash_timer = 0;
			ChangeState("idle");
		}
	}
	else {
		EngineTimer.setTimeSlower(1.f);
		change_color(VEC4(1, 1, 1, 1)); 
		omnidash_timer = 0;
		ChangeState("idle");
	}
	
}

void TCompPlayerController::dashing_state(float dt) {
	if (looking_left)
		move_player(false, false, dt, gravity);
	else
		move_player(true, false, dt, gravity);
	
	dashing_amount += 0.1;
	if (dashing_amount > dashing_max) {
		change_color(VEC4(1, 1, 1, 1));
		ChangeState("idle");
		dashing_amount = 0;
		speed_factor = speed_factor / dashing_speed;
	}
}

void TCompPlayerController::dead_state(float dt) {
}