
#include "mcv_platform.h"
#include "entity/entity_parser.h"
#include "comp_player_controller.h"
#include "comp_transform.h"
#include "entity/common_msgs.h"

DECL_OBJ_MANAGER("player_controller", TCompPlayerController);

void TCompPlayerController::MovePlayer(bool left) {
	TCompTransform *c_my_transform = get<TCompTransform>();
	VEC3 myPos = c_my_transform->getPosition();

	// Current orientation
	float current_yaw = 0.f;
	float current_pitch = 0.f;
	float amount_moved = speedFactor * delta;
	c_my_transform->getYawPitchRoll(&current_yaw, &current_pitch);

	//Detecto el teclado
	center.y = myPos.y;
	float distance = VEC3::Distance(myPos, center);
	VEC3 move_vector = center + myPos;
	c_my_transform->setPosition(center);
	
	current_yaw = left ? current_yaw + 0.1 * amount_moved : current_yaw - 0.1 * amount_moved;
	c_my_transform->setYawPitchRoll(current_yaw, current_pitch);
	VEC3 newPos = c_my_transform->getPosition() + (c_my_transform->getFront() * distance);
	c_my_transform->setYawPitchRoll(current_yaw, current_pitch);	

	TCompCollider* comp_collider = get<TCompCollider>();
	if (comp_collider && comp_collider->controller)
	{
		VEC3 delta_move = newPos - myPos;
		delta_move.y += -9.81*delta;
		comp_collider->controller->move(physx::PxVec3(delta_move.x, delta_move.y, delta_move.z), 0.f, delta, physx::PxControllerFilters());
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

void TCompPlayerController::IdleState() {
	const Input::TButton& bt = CEngine::get().getInput().host(Input::PLAYER_1).keyboard().key(VK_SPACE);
	if (bt.getsPressed()) {
		dashingAmount = 0;
		dashingMax = 3;
		speedFactor = speedFactor * dashingSpeed;
		ChangeState("dash");
	}

	if (isPressed('A')) {
		MovePlayer(false);
		lookingLeft = true;
		ChangeState("run");
	}
	if (isPressed('D')) {
		MovePlayer(true);
		lookingLeft = false;
		ChangeState("run");
	}
}

void TCompPlayerController::RunningState() {	
	// Compruebo si sigue corriendo
	if (isPressed('A')) {
		lookingLeft = true;
		MovePlayer(false);
	}
	if (isPressed('D')) {
		lookingLeft = false;
		MovePlayer(true);
	}
	// Si no sigue corriendo pasa a estado idle
	if (!isPressed('A') && !isPressed('D'))
		ChangeState("idle");

	// Si presiona la barra pasa a estado dashing
	const Input::TButton& bt = CEngine::get().getInput().host(Input::PLAYER_1).keyboard().key(VK_SPACE);
	if (bt.getsPressed()) {
		dashingAmount = 0;
		dashingMax = 10;
		speedFactor = speedFactor * dashingSpeed;
		ChangeState("dash");
	}
}

void TCompPlayerController::JumpingState() {
	const Input::TButton& bt = CEngine::get().getInput().host(Input::PLAYER_1).keyboard().key(VK_SPACE);
	if (bt.getsPressed())
		ChangeState("dash");
}

void TCompPlayerController::OmniDashingState() {
}

void TCompPlayerController::DashingState() {
	if (lookingLeft)
		MovePlayer(false);
	else
		MovePlayer(true);
	
	dashingAmount += 0.1;
	if (dashingAmount > dashingMax) {
		ChangeState("idle");
		dashingAmount = 0;
		speedFactor = speedFactor / dashingSpeed;
	}
}

void TCompPlayerController::DeadState() {
}