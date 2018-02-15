#include "mcv_platform.h"
#include "entity/entity_parser.h"
#include "comp_player_controller.h"
#include "comp_transform.h"
#include "entity/common_msgs.h"

DECL_OBJ_MANAGER("player_controller", TCompPlayerController);

void TCompPlayerController::MovePlayer(bool left, float dt) {
	TCompTransform *c_my_transform = get<TCompTransform>();
	VEC3 myPos = c_my_transform->getPosition();
	assert(c_my_transform);
	// Current orientation
	float current_yaw = 0.f;
	float current_pitch = 0.f;
	float amount_moved = speedFactor * dt;
	c_my_transform->getYawPitchRoll(&current_yaw, &current_pitch);

	//Detecto el teclado
	center.y = myPos.y;
	float distance = VEC3::Distance(myPos, center);
	VEC3 move_vector = center + myPos;
	
	current_yaw = left ? current_yaw + 0.1 * amount_moved : current_yaw - 0.1 * amount_moved;
	c_my_transform->setYawPitchRoll(current_yaw, current_pitch);
	VEC3 newPos = center + (c_my_transform->getLeft() * distance);
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

void TCompPlayerController::debugInMenu() {
  ImGui::DragFloat("Speed", &speedFactor, 0.1f, 0.f, 20.f);
	ImGui::Text("State: %s", state.c_str());
	ImGui::Text("Dashing ammount: %f", dashingAmount);
}

void TCompPlayerController::load(const json& j, TEntityParseContext& ctx) {
  speedFactor = j.value("speed", 1.0f);
  center = VEC3(0.f, 0.f, 0.f);
	dashingSpeed = j.value("dashing_speed", 5);
	Init();
}

void TCompPlayerController::Init() {
	// insert all states in the map
	AddState("idle", (statehandler)&TCompPlayerController::IdleState);
	AddState("run", (statehandler)&TCompPlayerController::RunningState);
	AddState("jump", (statehandler)&TCompPlayerController::JumpingState);
	AddState("omni", (statehandler)&TCompPlayerController::OmniDashingState);
	AddState("dash", (statehandler)&TCompPlayerController::DashingState);
	AddState("dead", (statehandler)&TCompPlayerController::DeadState);

	// reset the state
	ChangeState("idle");
}

void TCompPlayerController::IdleState(float dt) {
	// Chequea el dash
	const Input::TButton& space = CEngine::get().getInput().host(Input::PLAYER_1).keyboard().key(VK_SPACE);
	if (space.getsPressed()) {
		dashingAmount = 0;
		dashingMax = 3;
		speedFactor = speedFactor * dashingSpeed;
		ChangeState("dash");
	}

	if (isPressed('A')) {
		MovePlayer(false, dt);
		lookingLeft = true;
		ChangeState("run");
	}
	if (isPressed('D')) {
		MovePlayer(true, dt);
		lookingLeft = false;
		ChangeState("run");
	}

	// Chequea el salto
	const Input::TButton& shift = CEngine::get().getInput().host(Input::PLAYER_1).keyboard().key(VK_LSHIFT);
	if (shift.getsPressed()) {
		ChangeState("jump");
	}
}

void TCompPlayerController::RunningState(float dt) {
	// Compruebo si sigue corriendo
	if (isPressed('A')) {
		lookingLeft = true;
		MovePlayer(false, dt);
	}
	if (isPressed('D')) {
		lookingLeft = false;
		MovePlayer(true, dt);
	}
	// Si no sigue corriendo pasa a estado idle
	if (!isPressed('A') && !isPressed('D'))
		ChangeState("idle");

	if (isPressed('O')) {
		EngineTimer.setTimeSlower(0.25f);
		ChangeState("omni");
	}

	// Chequea el salto
	const Input::TButton& shift = CEngine::get().getInput().host(Input::PLAYER_1).keyboard().key(VK_LSHIFT);
	if (shift.getsPressed()) {
		ChangeState("jump");
	}
	// Chequea el dash
	const Input::TButton& space = CEngine::get().getInput().host(Input::PLAYER_1).keyboard().key(VK_SPACE);
	if (space.getsPressed()) {
		dashingAmount = 0;
		dashingMax = 10;
		speedFactor = speedFactor * dashingSpeed;
		ChangeState("dash");
	}
}

void TCompPlayerController::JumpingState(float dt) {
	TCompCollider* comp_collider = get<TCompCollider>();
	TCompTransform *c_my_transform = get<TCompTransform>();
	assert(c_my_transform);
	VEC3 myPos = c_my_transform->getPosition();
	comp_collider->controller->move(physx::PxVec3(myPos.x, myPos.y + 5, myPos.z), 0.f, dt, physx::PxControllerFilters());
	ChangeState("idle");
}

void TCompPlayerController::OmniDashingState(float dt) {
	if (!isPressed('O')) {
		EngineTimer.setTimeSlower(1.f);
		ChangeState("idle");
	}
}

void TCompPlayerController::DashingState(float dt) {
	if (lookingLeft)
		MovePlayer(false, dt);
	else
		MovePlayer(true, dt);
	
	dashingAmount += 0.1;
	if (dashingAmount > dashingMax) {
		ChangeState("idle");
		dashingAmount = 0;
		speedFactor = speedFactor / dashingSpeed;
	}
}

void TCompPlayerController::DeadState(float dt) {
}