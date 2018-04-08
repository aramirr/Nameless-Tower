#include "mcv_platform.h"
#include "entity/entity_parser.h"
#include "comp_player_controller.h"
#include "components/juan/comp_transform.h"
#include "components/camera/comp_camera.h"
#include "components/ui/ui_mouse_pos.h"
#include "entity/common_msgs.h"
#include "components/fsm/comp_fsm.h"

DECL_OBJ_MANAGER("player_controller", TCompPlayerController);

void TCompPlayerController::debugInMenu() {
	//ImGui::Text("State: %s", state.c_str());
	//ImGui::Text("Can dash: %s", can_dash ? "Si" : "No");
	//ImGui::Text("Grounded: %s", is_grounded ? "Si" : "No");
	ImGui::DragFloat("X speed: %f", &x_speed_factor, 0.01f, 0.f, 5.f);
	ImGui::DragFloat("Y speed: %f", &y_speed_factor, 0.01f, 0.f, 100.f);
	ImGui::DragFloat("Gravity: %f", &gravity, 0.01f, 0.f, 200.f);
	ImGui::DragFloat("Jump speed: %f", &jump_speed, 0.01f, 0.f, 100.f);
	ImGui::DragFloat("Omnidash max: %f", &omnidashing_max_ammount, 0.1f, 0.f, 10.f);
	ImGui::DragFloat("Dash max: %f", &dashing_max, 0.05f, 0.f, 1.f);
}

void TCompPlayerController::load(const json& j, TEntityParseContext& ctx) {
	setEntity(ctx.current_entity);
	x_speed_factor = j.value("speed", 1.0f);
	dashing_max = j.value("dashing_max", 0.35f);
	gravity = j.value("gravity", 75.f);
	jump_speed = j.value("jump_speed", 12.6f);
	center = VEC3(0.f, 0.f, 0.f);
	tower_radius = j.value("tower_radius", 32.f);
	dashing_speed = j.value("dashing_speed", 3);
	omnidash_max_time = j.value("omnidash_max_time", 0.3f);
	omnidashing_max_ammount = j.value("omnidashing_max_ammount", 1.3f);
	jumping_death_height = j.value("jumping_death_height", 9.f);
	current_x_speed_factor = x_speed_factor; 
	is_grounded = true;
	looking_left = true;
	can_omni = true;
	can_dash = true;

  DT = 0.f;

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

void TCompPlayerController::change_mesh(int mesh_index) {
	TCompRender *my_render = getMyRender();
	//my_render->mesh = my_render->meshes_leo[mesh_index];
	for (int i = 0; i < my_render->meshes.size(); ++i) {
		my_render->meshes[i].enabled = false;
	}
	my_render->meshes[mesh_index].enabled = true;
	my_render->refreshMeshesInRenderManager();
}


void TCompPlayerController::move_player(bool left, bool change_orientation, float dt, float y_speed) {
	TCompTransform *c_my_transform = get<TCompTransform>();

	assert(c_my_transform);
	// Current orientation
	float current_yaw;
	float current_pitch;
	float amount_moved = current_x_speed_factor * dt;
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
			physx::PxControllerCollisionFlags flags = comp_collider->controller->move(physx::PxVec3(delta_move.x, delta_move.y, delta_move.z), 0.f, dt, physx::PxControllerFilters());
			if (flags.isSet(physx::PxControllerCollisionFlag::eCOLLISION_DOWN) && !is_grounded) {
				if (c_my_transform->getPosition().y - jumping_start_height > jumping_death_height) {
					TMsgSetFSMVariable deadMsg;
					deadMsg.variant.setName("dead");
					deadMsg.variant.setBool(true);
					CEntity* e = CHandle(this).getOwner();
					e->sendMsg(deadMsg);
				}
				y_speed_factor = 0;
				is_grounded = true;
				can_omni = true;
				can_dash = true;
				//MENSAJE				
				/*if (dashing_amount == 0) {
					ChangeState("idle");
				}
				if (state == "jump"){}
					ChangeState("idle");
					*/
			}
			else if (!flags.isSet(physx::PxControllerCollisionFlag::eCOLLISION_DOWN)) {
				is_grounded = false;
				jumping_start_height = c_my_transform->getPosition().y;
			}

			if (flags.isSet(physx::PxControllerCollisionFlag::eCOLLISION_SIDES)) {
				current_yaw = left ? current_yaw - 0.1f * amount_moved : current_yaw + 0.1f * amount_moved;
				c_my_transform->setYawPitchRoll(current_yaw, current_pitch);
			}
			/*else if (flags.isSet(physx::PxControllerCollisionFlag::eCOLLISION_UP)) {
				change_mesh(1);
				if (!isPressed('A') && !isPressed('D')) {
					change_mesh(1);
					can_dash = true;
					ChangeState("idle");
				}
				else {
					change_mesh(0);
					can_dash = true;
					ChangeState("run");
				}
			}
			if (state != "dash" && current_x_speed_factor != x_speed_factor) {
				current_x_speed_factor = x_speed_factor;
				change_mesh(1);
				ChangeState("idle");
			}*/
		}
		else
		{
			//Actualizo la posicion del transform
			c_my_transform->setPosition(newPos);
		}
	}
}

/*
void TCompPlayerController::initial_state(float dt) {
	TCompTransform *my_pos = getMyTransform();
	TCompCollider* comp_collider = get<TCompCollider>();
	if (comp_collider && comp_collider->controller) {
		if (checkpoint.x)
			comp_collider->controller->setPosition(physx::PxExtendedVec3(checkpoint.x, checkpoint.y, checkpoint.z));
		else
			checkpoint = my_pos->getPosition();
	}

	
	my_pos->lookAt(my_pos->getPosition(), center);
	float y, p, r;
	my_pos->getYawPitchRoll(&y, &p, &r);
	y -= deg2rad(90);
	my_pos->setYawPitchRoll(y, 0, 0);

	looking_left = my_pos->isInLeft(center) ? false : true;
	change_mesh(1);
	ChangeState("idle");
}

void TCompPlayerController::idle_state(float dt) {
	TCompCollider* comp_collider = get<TCompCollider>();

	//---------- CHEAT de EDU el manco ----------
	if (isPressed('O')) {
		if (comp_collider && comp_collider->controller) {
			VEC3 cheat_position = VEC3(30.364f, 45.759f, 9.768f);
			comp_collider->controller->setPosition(physx::PxExtendedVec3(cheat_position.x, cheat_position.y, cheat_position.z));
		}
		ChangeState("initial");
	}
	if (isPressed('E')) {
		TEntityParseContext ctx;
		ctx.entity_starting_the_parse = CHandle(this).getOwner();
		ctx.root_transform = *(TCompTransform*)get<TCompTransform>();
		if (parseScene("data/prefabs/windstrike.prefab", ctx)) {
			assert(!ctx.entities_loaded.empty());
		}
	}
	//----------------------------------


	TCompTransform *c_my_transform = getMyTransform();
	const Input::TButton& dash = CEngine::get().getInput().host(Input::PLAYER_1).keyboard().key(VK_LSHIFT);
	const Input::TButton& space = CEngine::get().getInput().host(Input::PLAYER_1).keyboard().key(VK_SPACE);

	float y_speed = (y_speed_factor * DT) - (gravity * DT * DT / 2);
	if (!is_grounded)
		y_speed_factor -= gravity * DT / 2;

	// Chequea el salto
	if (space.getsPressed() && is_grounded) {
		is_grounded = false;
		jumping_start_height = c_my_transform->getPosition().y;
		change_mesh(2);
		TMsgJump msg_jump;
		msg_jump.jump_position = c_my_transform->getPosition();
		CEntity* e_runner = (CEntity*)getEntityByName("Boss Runner");
		e_runner->sendMsg(msg_jump);
		y_speed_factor = jump_speed;
		ChangeState("jump");
	} 
	// Chequea el dash
	else if (dash.getsPressed() && can_dash) {
		dashing_amount = 0;
		current_x_speed_factor = x_speed_factor * dashing_speed;
		change_mesh(4);
		can_dash = false;
		ChangeState("dash");
	}

	else {
		if (comp_collider && comp_collider->controller)
		{
			physx::PxControllerCollisionFlags flags = comp_collider->controller->move(physx::PxVec3(0, y_speed, 0), 0.f, DT, physx::PxControllerFilters());
			if (flags.isSet(physx::PxControllerCollisionFlag::eCOLLISION_DOWN) && !is_grounded) {
				if (jumping_start_height - c_my_transform->getPosition().y > jumping_death_height) {
					change_mesh(5);
					ChangeState("dead");
				}
				y_speed_factor = 0;
				is_grounded = true;
				can_omni = true;
				can_dash = true;
			}
			else if (!flags.isSet(physx::PxControllerCollisionFlag::eCOLLISION_DOWN) && is_grounded) {
				is_grounded = false;
				jumping_start_height = c_my_transform->getPosition().y;
			}
		}

		if (isPressed('A')) {
			if (!looking_left) {
				looking_left = true;
				move_player(false, true, DT, y_speed);
			}
			else {
				move_player(false, false, DT, y_speed);
			}
			change_mesh(0);
			ChangeState("run");
		}
		if (isPressed('D')) {
			if (!looking_left) {
				move_player(true, false, DT, y_speed);
			}
			else {
				looking_left = false;
				move_player(true, true, DT, y_speed);
			}
			change_mesh(0);
			ChangeState("run");
		}

		// Chequea el omnidash si es que esta en bajada
		const Input::TButton& omni = CEngine::get().getInput().host(Input::PLAYER_1).mouse().button(Input::MOUSE_LEFT);
		if (omni.getsPressed() && !is_grounded && can_omni) {
			EngineTimer.setTimeSlower(0.25f);
			can_omni = false;
			change_mesh(3);
			ChangeState("omni");
		}
	}
}

void TCompPlayerController::running_state(float dt) {
	const Input::TButton& dash = CEngine::get().getInput().host(Input::PLAYER_1).keyboard().key(VK_LSHIFT);
	const Input::TButton& space = CEngine::get().getInput().host(Input::PLAYER_1).keyboard().key(VK_SPACE);

	// Chequea el salto	
	if (space.getsPressed() && is_grounded) {
		TCompTransform *c_my_transform = get<TCompTransform>();
		is_grounded = false;
		jumping_start_height = c_my_transform->getPosition().y;
		change_mesh(2);
		TMsgJump msg_jump;
		msg_jump.jump_position = c_my_transform->getPosition();
		CEntity* e_runner = (CEntity*)getEntityByName("Boss Runner");
		e_runner->sendMsg(msg_jump);
		y_speed_factor = jump_speed;
		ChangeState("jump");
	}

	// Chequea el dash	
	else if (dash.getsPressed() && can_dash) {
		dashing_amount = 0;
		current_x_speed_factor = x_speed_factor * dashing_speed;
		change_mesh(4);
		can_dash = false;
		ChangeState("dash");
	}
	else {
		// Compruebo si sigue corriendo
		float y_speed = (y_speed_factor * DT) - (gravity * DT * DT / 2);
		if (!is_grounded)
			y_speed_factor -= gravity * DT / 2;
		if (isPressed('A')) {
			if (!looking_left) {
				looking_left = true;
				move_player(false, true, DT, y_speed);
			}
			else {
				move_player(false, false, DT, y_speed);
			}
		}
		else if (isPressed('D')) {
			if (!looking_left) {
				move_player(true, false, DT, y_speed);
			}
			else {
				looking_left = false;
				move_player(true, true, DT, y_speed);
			}
		}
		// Si no sigue corriendo pasa a estado idle
		if (!isPressed('A') && !isPressed('D')) {
			change_mesh(1);
			ChangeState("idle");
		}

		// Chequea el omnidash si es que esta en bajada
		const Input::TButton& omni = CEngine::get().getInput().host(Input::PLAYER_1).mouse().button(Input::MOUSE_LEFT);
		if (omni.getsPressed() && !is_grounded && can_omni) {
			EngineTimer.setTimeSlower(0.25f);
			can_omni = false;
			change_mesh(3);
			ChangeState("omni");
		}
	}	
}

void TCompPlayerController::jumping_state(float dt) {

	TCompCollider* comp_collider = get<TCompCollider>();
	TCompTransform *c_my_transform = get<TCompTransform>();
	assert(c_my_transform);
	VEC3 my_pos = c_my_transform->getPosition();
	VEC3 new_pos = my_pos;
	float y_speed;
	if (y_speed_factor > 0)
		y_speed = (y_speed_factor * DT) - (gravity * DT * DT / 2);
	else
		y_speed = (y_speed_factor * DT) - (gravity * DT * DT * 3);
	y_speed_factor -= gravity * DT / 2;
	new_pos.y += y_speed;

	// Chequea el dash		
	const Input::TButton& dash = CEngine::get().getInput().host(Input::PLAYER_1).keyboard().key(VK_LSHIFT);
	if (dash.getsPressed() && can_dash) {
		dashing_amount = 0;
		current_x_speed_factor = x_speed_factor * dashing_speed;
		change_mesh(4);
		can_dash = false;
		ChangeState("dash");
	}

	// Chequea movimiento
	if (isPressed('A')) {
		if (!looking_left) {
			looking_left = true;
			move_player(false, true, DT, y_speed);
		}
		else {
			move_player(false, false, DT, y_speed);
		}
	}
	else if (isPressed('D')) {
		if (!looking_left) {
			move_player(true, false, DT, y_speed);
		}
		else {
			looking_left = false;
			move_player(true, true, DT, y_speed);
		}
	}
	else {
		VEC3 delta_move = new_pos - my_pos;
		physx::PxControllerCollisionFlags flags = comp_collider->controller->move(physx::PxVec3(delta_move.x, delta_move.y, delta_move.z), 0.f, DT, physx::PxControllerFilters());
		if (flags.isSet(physx::PxControllerCollisionFlag::eCOLLISION_UP)) {
			change_mesh(1);
			y_speed_factor = 0;
			ChangeState("idle");
		}
		else if (flags.isSet(physx::PxControllerCollisionFlag::eCOLLISION_DOWN)) {
			change_mesh(1);
			y_speed_factor = 0;
			ChangeState("idle");
		}
	}

	// Chequea si se presiona el omnidash
	const Input::TButton& omni = CEngine::get().getInput().host(Input::PLAYER_1).mouse().button(Input::MOUSE_LEFT);
	if (omni.getsPressed() && can_omni) {
		EngineTimer.setTimeSlower(0.25f);
		can_omni = false;
		change_mesh(3);
		ChangeState("omni");
	}
}

void TCompPlayerController::omnidashing_state(float dt) {
	omnidash_timer += DT;
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
		omnidashing_ammount = 0;
		CHandle h_e_camera = getEntityByName("the_camera");
		CEntity* e_camera = h_e_camera;
		TCompCamera* c_camera = e_camera->get< TCompCamera >();
		TCompTransform *c_my_transform = get<TCompTransform>();
		const Input::TInterface_Mouse& mouse = EngineInput.mouse();
		VEC3 my_pos = c_my_transform->getPosition();
		VEC3 player_position;
		c_camera->getScreenCoordsOfWorldCoord(my_pos, &player_position);
		omnidash_arrow = mouse._position - VEC2(player_position.x, player_position.y);
		omnidash_arrow.Normalize();
		y_speed_factor = 0;
		
		TEntityParseContext ctx;
		ctx.entity_starting_the_parse = CHandle(this).getOwner();
		ctx.root_transform = *(TCompTransform*)get<TCompTransform>();
		if (parseScene("data/prefabs/windstrike.prefab", ctx)) {
			assert(!ctx.entities_loaded.empty());
			TMsgChangeDirection MsgChangeDirection;
			VEC3 omni_vector = c_my_transform->getFront();
			if (looking_left)
				omni_vector *= omnidash_arrow.x;
			else
				omni_vector *= omnidash_arrow.x * -1;

			omni_vector.y += omnidash_arrow.y;
			MsgChangeDirection.new_direction = -omni_vector;
			CEntity *e = ctx.current_entity;
			e->sendMsg(MsgChangeDirection);
		}
		
		ChangeState("omni_jump");
	}
}

void TCompPlayerController::omnidashing_jump_state(float dt) {
	if (omnidashing_ammount < omnidashing_max_ammount / 20) {
		TCompCollider* comp_collider = get<TCompCollider>();
		TCompTransform *c_my_transform = get<TCompTransform>();
		VEC3 my_pos = c_my_transform->getPosition();
		omnidash_vector = c_my_transform->getFront();
		if (looking_left)
			omnidash_vector *= omnidash_arrow.x;
		else
			omnidash_vector *= omnidash_arrow.x * -1;

		omnidash_vector.y += omnidash_arrow.y;
		VEC3 new_pos;
		new_pos = my_pos + (omnidash_vector * ((jump_speed * 6 - gravity * DT) * DT));

		VEC3 centre = VEC3(0, new_pos.y, 0);
		float d = VEC3::Distance(centre, new_pos);
		d = (tower_radius) / d;
		new_pos.x = new_pos.x * d;
		new_pos.z = new_pos.z * d;

		VEC3 delta_move = new_pos - my_pos;
		omnidashing_ammount += DT;

		float current_yaw;
		float current_pitch;
		float amount_moved = current_x_speed_factor * DT;
		c_my_transform->getYawPitchRoll(&current_yaw, &current_pitch);
		current_yaw = current_yaw - (1.15f * omnidash_arrow.x * amount_moved);
		c_my_transform->setYawPitchRoll(current_yaw, current_pitch);
		physx::PxControllerCollisionFlags flags = comp_collider->controller->move(physx::PxVec3(delta_move.x, delta_move.y, delta_move.z), 0.f, dt, physx::PxControllerFilters());
		if (flags.isSet(physx::PxControllerCollisionFlag::eCOLLISION_UP) || flags.isSet(physx::PxControllerCollisionFlag::eCOLLISION_SIDES) || flags.isSet(physx::PxControllerCollisionFlag::eCOLLISION_DOWN)) {
			EngineTimer.setTimeSlower(1.f);
			omnidash_timer = 0;
			omnidashing_ammount = 0;
			change_mesh(1);
			ChangeState("idle");
		}
	}
	else {
		EngineTimer.setTimeSlower(1.f);
		omnidash_timer = 0;
		omnidashing_ammount = 0;
		change_mesh(1);
		ChangeState("idle");
	}
}

void TCompPlayerController::dashing_state(float dt) {
	if (looking_left)
		move_player(false, false, DT, 0);
	else
		move_player(true, false, DT, 0);

	dashing_amount += DT;
	if (dashing_amount > dashing_max) {
		change_mesh(1);
		dashing_amount = 0;
		current_x_speed_factor = x_speed_factor / dashing_speed;
		ChangeState("idle");
	}
}

void TCompPlayerController::dead_state(float dt) {
	TCompTransform *c_my_transform = getMyTransform();
	TCompCollider* comp_collider = get<TCompCollider>();
	if (isPressed('P')) {		
		if (comp_collider && comp_collider->controller) {
			comp_collider->controller->setPosition(physx::PxExtendedVec3(checkpoint.x, checkpoint.y, checkpoint.z));
		}
		//Engine.getModules().changeGameState("test_axis");
		ChangeState("initial");
		return;
	}
	else {
		float y_speed = (y_speed_factor * DT) - (gravity * DT * DT / 2);
		if (!is_grounded)
			y_speed_factor -= gravity * DT / 2;
		if (comp_collider && comp_collider->controller)
		{
			physx::PxControllerCollisionFlags flags = comp_collider->controller->move(physx::PxVec3(0, y_speed, 0), 0.f, DT, physx::PxControllerFilters());
			if (flags.isSet(physx::PxControllerCollisionFlag::eCOLLISION_DOWN) && !is_grounded) {
				if (c_my_transform->getPosition().y - jumping_start_height > jumping_death_height) {
					change_mesh(5);
					ChangeState("dead");
				}
				y_speed_factor = 0;
				is_grounded = true;
				can_omni = true;
				can_dash = true;

			}
			else if (!flags.isSet(physx::PxControllerCollisionFlag::eCOLLISION_DOWN) && is_grounded) {
				is_grounded = false;
			}
		}
	}	
}

void TCompPlayerController::registerMsgs() {
	DECL_MSG(TCompPlayerController, TMsgKillPlayer, killPlayer);
	DECL_MSG(TCompPlayerController, TMsgCheckpoint, setCheckpoint);
}

void TCompPlayerController::killPlayer(const TMsgKillPlayer& msg)
{
	change_mesh(5);
	ChangeState("dead");
}
*/

void TCompPlayerController::setCheckpoint(const TMsgCheckpoint& msg)
{
   	checkpoint = msg.appearing_position;
}
