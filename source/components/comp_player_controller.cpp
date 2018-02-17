#include "mcv_platform.h"
#include "entity/entity_parser.h"
#include "comp_player_controller.h"
#include "comp_transform.h"
#include "entity/common_msgs.h"

DECL_OBJ_MANAGER("player_controller", TCompPlayerController);

void TCompPlayerController::MovePlayer(bool left, bool change_orientation, float dt) {
	TCompTransform *c_my_transform = get<TCompTransform>();
	VEC3 myPos = c_my_transform->getPosition();
	assert(c_my_transform);
	// Current orientation
	float current_yaw;
	float current_pitch;
	float amount_moved = speedFactor * dt;
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
			delta_move.y += -9.81*dt;
			comp_collider->controller->move(physx::PxVec3(delta_move.x, delta_move.y, delta_move.z), 0.f, dt, physx::PxControllerFilters());
		}
		else
		{
			//Actualizo la posicion del transform
			c_my_transform->setPosition(newPos);
		}
	}		
}

void TCompPlayerController::debugInMenu() {
  ImGui::DragFloat("Speed", &speedFactor, 0.1f, 0.f, 20.f);
	ImGui::Text("State: %s", state.c_str());
	ImGui::Text("Dashing ammount: %f", dashingAmount);
}

void TCompPlayerController::load(const json& j, TEntityParseContext& ctx) {
	setEntity(ctx.current_entity);
    speedFactor = j.value("speed", 1.0f);
    center = VEC3(0.f, 0.f, 0.f);
	tower_radius = j.value("tower_radius", 15.f);
	dashingSpeed = j.value("dashing_speed", 5);
	max_jump = j.value("max_jump", 5);

	Init();	
}

void TCompPlayerController::Init() {
	// insert all states in the map
	AddState("idle", (statehandler)&TCompPlayerController::IdleState);
	AddState("initial", (statehandler)&TCompPlayerController::InitialState);
	AddState("run", (statehandler)&TCompPlayerController::RunningState);
	AddState("jump", (statehandler)&TCompPlayerController::JumpingState);
	AddState("omni", (statehandler)&TCompPlayerController::OmniDashingState);
	AddState("dash", (statehandler)&TCompPlayerController::DashingState);
	AddState("dead", (statehandler)&TCompPlayerController::DeadState);

	// reset the state
	ChangeState("initial");

}

void TCompPlayerController::InitialState(float dt) {
	TCompTransform *my_pos = getMyTransform();
	my_pos->lookAt(my_pos->getPosition(), center);
	float y, p, r;
	my_pos->getYawPitchRoll(&y, &p, &r);
	y += deg2rad(90);
	my_pos->setYawPitchRoll(y, p, r);

	lookingLeft = my_pos->isInLeft(center) ? false : true;
	ChangeState("idle");
}

void TCompPlayerController::IdleState(float dt) {	
	TCompCollider* comp_collider = get<TCompCollider>();
	TCompTransform *c_my_transform = getMyTransform();
	if (comp_collider && comp_collider->controller)
	{
		comp_collider->controller->move(physx::PxVec3(0, -9.81*dt, 0), 0.f, dt, physx::PxControllerFilters());
	}
	// Chequea el dash
	const Input::TButton& dash = CEngine::get().getInput().host(Input::PLAYER_1).keyboard().key(VK_LSHIFT);
	if (dash.getsPressed()) {
		dashingAmount = 0;
		dashingMax = 3;
		speedFactor = speedFactor * dashingSpeed;
		change_color(VEC4(0, 1, 1, 1));
		ChangeState("dash");
	}

	float y, p, r;
	if (isPressed('A')) {
		if (!lookingLeft) {			
			lookingLeft = true;
			MovePlayer(false, true, dt);
		}
		else {			
			MovePlayer(false, false, dt);
		}		
		ChangeState("run");
	}
	if (isPressed('D')) {
		if (!lookingLeft) {			
			MovePlayer(true, false, dt);
		}
		else {
			lookingLeft = false;
			MovePlayer(true, true, dt);
		}		
		ChangeState("run");
	}

	// Chequea el salto
	const Input::TButton& space = CEngine::get().getInput().host(Input::PLAYER_1).keyboard().key(VK_SPACE);
	if (space.getsPressed()) {
		change_color(VEC4(1, 0, 1, 1));
		jump_end = c_my_transform->getPosition().y + max_jump;
		ChangeState("jump");
	}
}

void TCompPlayerController::RunningState(float dt) {
	// Compruebo si sigue corriendo

	if (isPressed('A')) {
		if (!lookingLeft) {
			lookingLeft = true;
			MovePlayer(false, true, dt);
		}
		else {
			MovePlayer(false, false, dt);
		}		
	}
	if (isPressed('D')) {
		if (!lookingLeft) {
			MovePlayer(true, false, dt);
		}
		else {
			lookingLeft = false;
			MovePlayer(true, true, dt);
		}		
	}
	// Si no sigue corriendo pasa a estado idle
	if (!isPressed('A') && !isPressed('D')){
		change_color(VEC4(1, 1, 1, 1));
		ChangeState("idle");
	}

	if (isPressed('O')) {
		change_color(VEC4(0, 1, 0, 1));
		EngineTimer.setTimeSlower(0.25f);
		ChangeState("omni");
	}

	// Chequea el salto
	const Input::TButton& space = CEngine::get().getInput().host(Input::PLAYER_1).keyboard().key(VK_SPACE);
	if (space.getsPressed()) {
		change_color(VEC4(1, 0, 1, 1));
		TCompTransform *c_my_transform = get<TCompTransform>();
		jump_end = c_my_transform->getPosition().y + max_jump;
		ChangeState("jump");
	}
	// Chequea el dash
	const Input::TButton& dash = CEngine::get().getInput().host(Input::PLAYER_1).keyboard().key(VK_LSHIFT);
	if (dash.getsPressed()) {
		dashingAmount = 0;
		dashingMax = 10;
		speedFactor = speedFactor * dashingSpeed;
		change_color(VEC4(0, 1, 1, 1));
		ChangeState("dash");
	}
}

void TCompPlayerController::JumpingState(float dt) {
	if (isPressed('A')) {
		MovePlayer(false, false, dt);
		lookingLeft = true;
	}
	if (isPressed('D')) {
		MovePlayer(true, false, dt);
		lookingLeft = false;
	}
	TCompCollider* comp_collider = get<TCompCollider>();
	TCompTransform *c_my_transform = get<TCompTransform>();
	assert(c_my_transform);
	VEC3 my_pos = c_my_transform->getPosition();
	VEC3 new_pos = my_pos;
	new_pos.y += 50 * dt;		
	VEC3 delta_move = new_pos - my_pos;
	if (new_pos.y < jump_end)
		comp_collider->controller->move(physx::PxVec3(delta_move.x, delta_move.y, delta_move.z), 0.f, dt, physx::PxControllerFilters());
	else {
		change_color(VEC4(1, 1, 1, 1));
		ChangeState("idle");
	}
}

void TCompPlayerController::OmniDashingState(float dt) {
	if (!isPressed('O')) {
		EngineTimer.setTimeSlower(1.f);
		change_color(VEC4(1, 1, 1, 1));
		ChangeState("idle");
	}
}

void TCompPlayerController::DashingState(float dt) {
	if (lookingLeft)
		MovePlayer(false, false, dt);
	else
		MovePlayer(true, false, dt);
	
	dashingAmount += 0.1;
	if (dashingAmount > dashingMax) {
		change_color(VEC4(1, 1, 1, 1));
		ChangeState("idle");
		dashingAmount = 0;
		speedFactor = speedFactor / dashingSpeed;
	}
}

void TCompPlayerController::DeadState(float dt) {
}