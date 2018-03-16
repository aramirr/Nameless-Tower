#include "mcv_platform.h"
#include "bt_runner.h"
#include "entity/entity_parser.h"

DECL_OBJ_MANAGER("bt_runner", bt_runner);

void bt_runner::on_player_jump(const TMsgJump& msg) {
	if (run) jump_positions.push(msg.jump_position);
}

void bt_runner::stop(const TMsgRunnerStop& msg) {
	freeze = true;
}

//Se tiene que añadir el appear directamente en IDLE
void bt_runner::appear(const TMsgRunnerAppear& msg) {
	freeze = false;
	run = true;
	appearing_position = msg.appearing_position;
}

void bt_runner::registerMsgs() {
	DECL_MSG(bt_runner, TMsgJump, on_player_jump);
	DECL_MSG(bt_runner, TMsgRunnerAppear, appear);
	DECL_MSG(bt_runner, TMsgRunnerStop, stop);
}

void bt_runner::load(const json& j, TEntityParseContext& ctx) {
	setEntity(ctx.current_entity);
	chase_distance = j.value("chase_distance", 3.0f);
	attack_distance = j.value("attack_distance", 1.0f);
	speed_factor = j.value("speed_factor", 5.0f);
	tower_radius = j.value("tower_radius", 32.0f);
	gravity = j.value("gravity", 16.5f);
	jump_speed = j.value("jump_speed", 25.8f);
	jump_altitude = j.value("jump_altitude", 5.0f);

  create("runner");
}

void bt_runner::debugInMenu() {
	bt::debugInMenu();
	string state;
	if (current == NULL)
		state = "null";
	else state = current->getName();
	ImGui::Text("State %s", state);
	ImGui::Text("Distance player %f", distance_to_player);
	ImGui::DragFloat("Chase Distance: %f", &chase_distance, 0.01f, 0.f, 100.f);
	ImGui::DragFloat("Attack Distance: %f", &attack_distance, 0.01f, 0.f, 100.f);
	ImGui::DragFloat("Speed: %f", &speed_factor, 0.01f, 0.f, 100.f);
	ImGui::DragFloat("Jump speed: %f", &jump_speed, 0.01f, 0.f, 100.f);
	ImGui::DragFloat("Jump altitude: %f", &jump_altitude, 0.01f, 0.f, 100.f);
	ImGui::DragFloat("Gravity: %f", &gravity, 0.01f, 0.f, 100.f);
	ImGui::DragFloat("Tower Radius: %f", &tower_radius, 0.01f, 0.f, 100.f);
}

void bt_runner::create(string s)
{
name=s;
btnode* r = createRoot("runner", PRIORITY, NULL, NULL);
addChild("runner", "idle_p", PRIORITY, (btcondition)&bt_runner::conditionIdle, NULL);
addChild("idle_p", "attack", ACTION, (btcondition)&bt_runner::conditionAttack, (btaction)&bt_runner::actionAttack);
addChild("idle_p", "disappear", ACTION, (btcondition)&bt_runner::conditionDisappear, (btaction)&bt_runner::actionDisappear);
addChild("idle_p", "idle_a", ACTION, NULL, (btaction)&bt_runner::actionIdle);
addChild("runner", "run", SEQUENCE, (btcondition)&bt_runner::conditionRun, NULL);
addChild("run", "appear", ACTION, NULL, (btaction)&bt_runner::actionAppear);
addChild("run", "chase_s", SEQUENCE, NULL, NULL);
addChild("chase_s", "chase", ACTION, NULL, (btaction)&bt_runner::actionChase);
addChild("chase_s", "chase_other", PRIORITY, NULL, NULL);
addChild("chase_other", "attack_2", ACTION, (btcondition)&bt_runner::conditionAttack, (btaction)&bt_runner::actionAttack);
addChild("chase_other", "disappear_2", ACTION, (btcondition)&bt_runner::conditionDisappear, (btaction)&bt_runner::actionDisappear);
addChild("runner", "hide", ACTION, NULL, (btaction)&bt_runner::actionHide);

}

//----- ACTIONS -----
int bt_runner::actionDisappear() {
	TCompRender *my_render = getMyRender();
	my_render->is_active = false;
	TCompTransform *my_transform = getMyTransform();
	TCompCollider *comp_collider = getMyCollider();
	comp_collider->controller->setPosition(physx::PxExtendedVec3(tower_center.x, tower_center.y, tower_center.z));
	run = false;
	freeze = false;
	return LEAVE;
};

int bt_runner::actionIdle() {
	change_mesh(1);
	return LEAVE;
};

int bt_runner::actionAppear() {
	TCompTransform* my_transform = getMyTransform();
	TCompCollider* my_collider = getMyCollider();
	my_collider->controller->setPosition(physx::PxExtendedVec3(appearing_position.x, appearing_position.y, appearing_position.z));
	tower_center.y = appearing_position.y;
	my_transform->lookAt(appearing_position, tower_center);
	float y, p, r;
	my_transform->getYawPitchRoll(&y, &p, &r);
	y -= deg2rad(90);
	my_transform->setYawPitchRoll(y, p, r);
	going_right = my_transform->isInLeft(tower_center);
	TCompRender *my_render = getMyRender();
	my_render->is_active = true;
	return LEAVE;
};

int bt_runner::actionAttack() {
	CEntity *player = (CEntity *)getEntityByName("The Player");
	TMsgKillPlayer kill;
	player->sendMsg(kill);
	change_mesh(0);
	return LEAVE;
};

int bt_runner::actionChase() {
	TCompTransform *c_my_transform = getMyTransform();
	VEC3 myPos = c_my_transform->getPosition();
	CEntity *player = (CEntity *)getEntityByName("The Player");
	TCompTransform *c_p_transform = player->get<TCompTransform>();
	VEC3 ppos = c_p_transform->getPosition();
	if (jumping) return jumping_state();
	if (!jump_positions.empty() && VEC3::Distance(myPos, jump_positions.front()) < 1.f) {
		jumping = true;
		jump_end = c_my_transform->getPosition().y + jump_altitude;
		jump_positions.pop();
		return jumping_state();
	}

	float current_yaw;
	float current_pitch;
	float amount_moved = speed_factor * DT;
	c_my_transform->getYawPitchRoll(&current_yaw, &current_pitch);

	tower_center.y = myPos.y;

	if (!c_my_transform->isInFront(ppos)) {
		current_yaw = going_right ? current_yaw - deg2rad(180) : current_yaw + deg2rad(180);
		float debug = rad2deg(current_yaw);
		going_right = !going_right;
		c_my_transform->setYawPitchRoll(current_yaw, current_pitch);
	}
	else {
		current_yaw = going_right ? current_yaw + 0.1f * amount_moved : current_yaw - 0.1f * amount_moved;
		c_my_transform->setYawPitchRoll(current_yaw, current_pitch);
		VEC3 aux_vector = going_right ? -1 * c_my_transform->getLeft() : c_my_transform->getLeft();
		VEC3 newPos = tower_center + (aux_vector * tower_radius);
		c_my_transform->setYawPitchRoll(current_yaw, current_pitch);
		TCompCollider* comp_collider = get<TCompCollider>();
		if (comp_collider && comp_collider->controller)
		{
			VEC3 delta_move = newPos - myPos;
			distance_to_player = VEC3::Distance(myPos, ppos);
			delta_move.y += -10 * DT;
			physx::PxControllerCollisionFlags flags = comp_collider->controller->move(physx::PxVec3(delta_move.x, delta_move.y, delta_move.z), 0.f, DT, physx::PxControllerFilters());
			if (flags.isSet(physx::PxControllerCollisionFlag::eCOLLISION_SIDES)) {
				current_yaw = going_right ? current_yaw - 0.1f * amount_moved : current_yaw + 0.1f * amount_moved;
				c_my_transform->setYawPitchRoll(current_yaw, current_pitch);
			}
		}
	}
	change_mesh(2);
	distance_to_player = VEC3::Distance(myPos, ppos);
	if ((distance_to_player < attack_distance) or (distance_to_player > chase_distance + 5.f)) {
		return LEAVE;
	}
	else return STAY;
};

int bt_runner::actionHide() {
	return LEAVE;
};

//----- CONDITIONS -----
bool bt_runner::conditionIdle() {
	return freeze;
};


bool bt_runner::conditionDisappear() {
	TCompTransform *c_my_transform = getMyTransform();
	VEC3 myPos = c_my_transform->getPosition();
	CEntity *player = (CEntity *)getEntityByName("The Player");
	TCompTransform *c_p_transform = player->get<TCompTransform>();
	VEC3 ppos = c_p_transform->getPosition();
	distance_to_player = VEC3::Distance(myPos, ppos);

	return (distance_to_player > chase_distance + 5.f);
};

bool bt_runner::conditionRun() {
	return run;
};

bool bt_runner::conditionAttack() {
	TCompTransform *c_my_transform = getMyTransform();
	VEC3 myPos = c_my_transform->getPosition();
	CEntity *player = (CEntity *)getEntityByName("The Player");
	TCompTransform *c_p_transform = player->get<TCompTransform>();
	VEC3 ppos = c_p_transform->getPosition();
	distance_to_player = VEC3::Distance(myPos, ppos);

	return (distance_to_player < attack_distance);
};




//----- EXTRA -----
int bt_runner::jumping_state() {
	TCompTransform *c_my_transform = get<TCompTransform>();
	VEC3 my_pos = c_my_transform->getPosition();
	VEC3 new_pos = my_pos;
	float y_speed;
	if (y_speed_factor > 0)
		y_speed = (y_speed_factor * DT) - (gravity * DT * DT / 2);
	else
		y_speed = (y_speed_factor * DT) - (gravity * DT * DT * 3);
	y_speed_factor -= gravity * DT / 2;
	new_pos.y += y_speed;

	tower_center.y = my_pos.y;

	if (new_pos.y < jump_end) {
		float current_yaw, current_pitch;
		c_my_transform->getYawPitchRoll(&current_yaw, &current_pitch);
		float amount_moved = speed_factor * DT;
		current_yaw = going_right ? current_yaw + 0.1f * amount_moved : current_yaw - 0.1f * amount_moved;
		c_my_transform->setYawPitchRoll(current_yaw, current_pitch);
		VEC3 aux_vector = going_right ? -1 * c_my_transform->getLeft() : c_my_transform->getLeft();
		new_pos = tower_center + (aux_vector * tower_radius);
		c_my_transform->setYawPitchRoll(current_yaw, current_pitch);
		TCompCollider* comp_collider = get<TCompCollider>();
		if (comp_collider && comp_collider->controller)
		{
			VEC3 delta_move = new_pos - my_pos;
			delta_move.y += -(-(jump_speed - gravity * DT)) * DT;
			physx::PxControllerCollisionFlags flags = comp_collider->controller->move(physx::PxVec3(delta_move.x, delta_move.y, delta_move.z), 0.f, DT, physx::PxControllerFilters());
			if (flags.isSet(physx::PxControllerCollisionFlag::eCOLLISION_SIDES)) {
				current_yaw = going_right ? current_yaw - 0.1f * amount_moved : current_yaw + 0.1f * amount_moved;
				c_my_transform->setYawPitchRoll(current_yaw, current_pitch);
			}
			if (flags.isSet(physx::PxControllerCollisionFlag::eCOLLISION_UP)) {
				jumping = false;
			}
		}
	}
	else {
		jumping = false;
	}
	change_mesh(3);
	return STAY;
}