#include "mcv_platform.h"
#include "entity/entity_parser.h"
#include "comp_player_controller.h"
#include "comp_transform.h"
#include "ui/ui_mouse_pos.h"
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
		TCompCollider* comp_collider = get<TCompCollider>();
		if (comp_collider && comp_collider->controller)
		{
			VEC3 delta_move = newPos - myPos;
			delta_move.y += -gravity*dt;
			physx::PxControllerCollisionFlags flags = comp_collider->controller->move(physx::PxVec3(delta_move.x, delta_move.y, delta_move.z), 0.f, dt, physx::PxControllerFilters());
			if (flags.isSet(physx::PxControllerCollisionFlag::eCOLLISION_DOWN) && !is_grounded) {
				is_grounded = true;
				can_omni = true;
			} 
			if (flags.isSet(physx::PxControllerCollisionFlag::eCOLLISION_SIDES)) {
				current_yaw = left ? current_yaw - 0.1 * amount_moved : current_yaw + 0.1 * amount_moved;
				c_my_transform->setYawPitchRoll(current_yaw, current_pitch);
			} else if (flags.isSet(physx::PxControllerCollisionFlag::eCOLLISION_UP)) {
				change_color(VEC4(1, 1, 1, 1));
				change_mesh(1);
				ChangeState("idle");
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
	dashing_max = j.value("dashing_max", 15.0f);
	gravity = j.value("gravity", 16.5f);
	jump_speed = j.value("jump_speed", 25.8f);
  center = VEC3(0.f, 0.f, 0.f);
	tower_radius = j.value("tower_radius", 15.f);
	dashing_speed = j.value("dashing_speed", 3);
	max_jump = j.value("max_jump", 5);
	omnidash_max_time = j.value("omnidash_max_time", 0.3);
	omnidashing_max_ammount = j.value("omnidashing_max_ammount", 140);
	is_grounded = true;
	can_omni = true;
	can_dash = true;

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
	AddState("omni_jump", (statehandler)&TCompPlayerController::omnidashing_jump_state);

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
	change_mesh(1);
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
			can_omni = true;
		}
	}
	// Chequea el dash
	const Input::TButton& dash = CEngine::get().getInput().host(Input::PLAYER_1).keyboard().key(VK_LSHIFT);
	if (dash.getsPressed()) {
		dashing_amount = 0;
		speed_factor = speed_factor * dashing_speed;
		change_color(VEC4(0, 1, 1, 1));
		change_mesh(4);
		ChangeState("dash");
	}

	// Chequea el movimiento
	float y, p, r;
	if (isPressed('A')) {
		if (!looking_left) {			
			looking_left = true;
			move_player(false, true, dt, gravity);
		}
		else {			
			move_player(false, false, dt, gravity);
		}		
		change_mesh(0);
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
		change_mesh(0);
		ChangeState("run");
	}

	// Chequea el salto
	const Input::TButton& space = CEngine::get().getInput().host(Input::PLAYER_1).keyboard().key(VK_SPACE);
	if (space.getsPressed() && is_grounded) {
		change_color(VEC4(1, 0, 1, 1));
		jump_end = c_my_transform->getPosition().y + max_jump;
		is_grounded = false;
		change_mesh(2);
		TMsgJump msg_jump;
		msg_jump.jump_position = c_my_transform->getPosition();
		CEntity* e_runner = (CEntity*)getEntityByName("Boss Runner");
		e_runner->sendMsg(msg_jump);
		ChangeState("jump");
	}

	// Chequea el omnidash si es que esta en bajada
	const Input::TButton& omni = CEngine::get().getInput().host(Input::PLAYER_1).mouse().button(Input::MOUSE_LEFT);
	if (omni.getsPressed() && !is_grounded && can_omni) {
		change_color(VEC4(0, 1, 0, 1));
		EngineTimer.setTimeSlower(0.25f);
		can_omni = false;
		change_mesh(3);
		ChangeState("omni");
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
		change_mesh(1);
		ChangeState("idle");
	}

	// Chequea el salto
	const Input::TButton& space = CEngine::get().getInput().host(Input::PLAYER_1).keyboard().key(VK_SPACE);
	if (space.getsPressed() && is_grounded) {
		change_color(VEC4(1, 0, 1, 1));
		TCompTransform *c_my_transform = get<TCompTransform>();
		jump_end = c_my_transform->getPosition().y + max_jump;
		is_grounded = false;
		change_mesh(2);
		TMsgJump msg_jump;
		msg_jump.jump_position = c_my_transform->getPosition();
		CEntity* e_runner = (CEntity*)getEntityByName("Boss Runner");
		e_runner->sendMsg(msg_jump);
		ChangeState("jump");
	}

	// Chequea el dash
	const Input::TButton& dash = CEngine::get().getInput().host(Input::PLAYER_1).keyboard().key(VK_LSHIFT);
	if (dash.getsPressed()) {
		dashing_amount = 0;
		speed_factor = speed_factor * dashing_speed;
		change_color(VEC4(0, 1, 1, 1));
		change_mesh(4);
		ChangeState("dash");
	}

	// Chequea el omnidash si es que esta en bajada
	const Input::TButton& omni = CEngine::get().getInput().host(Input::PLAYER_1).mouse().button(Input::MOUSE_LEFT);
	if (omni.getsPressed() && !is_grounded && can_omni) {
		change_color(VEC4(0, 1, 0, 1));
		EngineTimer.setTimeSlower(0.25f);
		can_omni = false;
		change_mesh(3);
		ChangeState("omni");
	}
}

void TCompPlayerController::jumping_state(float dt) {
	
	TCompCollider* comp_collider = get<TCompCollider>();
	TCompTransform *c_my_transform = get<TCompTransform>();
	assert(c_my_transform);
	VEC3 my_pos = c_my_transform->getPosition();
	VEC3 new_pos = my_pos;
	new_pos.y += (jump_speed - gravity * dt) * dt;

	// Chequea que no llego a la altura maxima 
	if (new_pos.y < jump_end) {
		// Chequea el dash		
		const Input::TButton& dash = CEngine::get().getInput().host(Input::PLAYER_1).keyboard().key(VK_LSHIFT);
		if (dash.getsPressed()) {
			dashing_amount = 0;
			speed_factor = speed_factor * dashing_speed;
			change_color(VEC4(0, 1, 1, 1));
			change_mesh(4);
			ChangeState("dash");
		}

		// Chequea movimiento
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
			physx::PxControllerCollisionFlags flags = comp_collider->controller->move(physx::PxVec3(delta_move.x, delta_move.y, delta_move.z), 0.f, dt, physx::PxControllerFilters());
			if (flags.isSet(physx::PxControllerCollisionFlag::eCOLLISION_UP)) {
				change_color(VEC4(1, 1, 1, 1));
				change_mesh(1);
				ChangeState("idle");
			}
		}

		// Chequea si se presiona el omnidash
		const Input::TButton& omni = CEngine::get().getInput().host(Input::PLAYER_1).mouse().button(Input::MOUSE_LEFT);
		if (omni.getsPressed() && can_omni) {
			change_color(VEC4(0, 1, 0, 1));
			EngineTimer.setTimeSlower(0.25f);
			can_omni = false;
			change_mesh(3);
			ChangeState("omni");
		}
	}
	else {
		change_color(VEC4(1, 1, 1, 1));
		change_mesh(1);
		ChangeState("idle");
	}
}

void TCompPlayerController::omnidashing_state(float dt) {
	omnidash_timer += dt;
	bool change_state = false;
	// Chequea si hay que realizar el salto
	if (omnidash_timer < omnidash_max_time) {
		const Input::TButton& omni = CEngine::get().getInput().host(Input::PLAYER_1).mouse().button(Input::MOUSE_LEFT);
		if (!omni.isPressed()) {
			change_state = true;			
		}
	}
	else {
		change_state = true;		
	}
	// Salgo del omni dash
	if (change_state) {		
		TCompArrowUI *c_my_arrow = get<TCompArrowUI>();
		VEC3 omni_jump = c_my_arrow->unit_force_vector;
		omnidash_vector = omni_jump;
		//omnidash_vector.Normalize();
		omnidashing_ammount = 0;
		ChangeState("omni_jump");		
	}	
}

void TCompPlayerController::omnidashing_jump_state(float dt) {
	if (omnidashing_ammount < omnidashing_max_ammount * 1000 * dt) {
		TCompCollider* comp_collider = get<TCompCollider>();
		TCompTransform *c_my_transform = get<TCompTransform>();
		VEC3 my_pos = c_my_transform->getPosition();
		//VEC3 new_pos = my_pos + (omnidash_vector * ((jump_speed - gravity * dt) * dt));
		float x, y;
		VEC2 aux = VEC2(10, 15);
		aux.Normalize();
		omnidash_vector = c_my_transform->getFront();
		omnidash_vector.y += aux.y;
		VEC3 new_pos = my_pos + (omnidash_vector * aux.x * ((jump_speed * 5 - gravity * dt) * dt));

		VEC3 centre = VEC3(0, new_pos.y, 0);
		float d = VEC3::Distance(centre, new_pos);
		d = (tower_radius) / d;
		new_pos.x = new_pos.x * d;
		new_pos.z = new_pos.z * d;

		//new_pos.y += (jump_speed - gravity * dt) * dt;
		//omni_jump.y += (jump_speed - gravity * dt) * dt;
		VEC3 delta_move = new_pos - my_pos;		
		omnidashing_ammount += 0.1;

		float current_yaw;
		float current_pitch;
		float amount_moved = speed_factor * dt;
		c_my_transform->getYawPitchRoll(&current_yaw, &current_pitch);
		current_yaw = !looking_left ? current_yaw + (1.08 * aux.x * amount_moved) : current_yaw - (1.08 * aux.x * amount_moved);
		c_my_transform->setYawPitchRoll(current_yaw, current_pitch);
		comp_collider->controller->move(physx::PxVec3(delta_move.x, delta_move.y, delta_move.z), 0.f, dt, physx::PxControllerFilters());
	}
	else {
		EngineTimer.setTimeSlower(1.f);
		omnidash_timer = 0;
		omnidashing_ammount = 0;
		change_color(VEC4(1, 1, 1, 1));
		change_mesh(1);
		ChangeState("idle");
	}	
}

void TCompPlayerController::dashing_state(float dt) {
	if (looking_left)
		move_player(false, false, dt, 0);
	else
		move_player(true, false, dt, 0);
	
	dashing_amount += 0.1;
	if (dashing_amount > dashing_max * 1000 * dt) {
		change_color(VEC4(1, 1, 1, 1));
		change_mesh(1);
		ChangeState("idle");
		dashing_amount = 0;
		speed_factor = speed_factor / dashing_speed;
	}
}

void TCompPlayerController::dead_state(float dt) {
}