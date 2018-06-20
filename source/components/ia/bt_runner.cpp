#include "mcv_platform.h"
#include "bt_runner.h"
#include "entity/entity_parser.h"
#include "components/physics/controller_filter.h"
#include "components/physics/query_filter.h"

DECL_OBJ_MANAGER("bt_runner", bt_runner);


//Se tiene que añadir el appear directamente en IDLE
void bt_runner::appear(const TMsgRunnerAppear& msg) {
    b_appear = true;
    appearing_position = msg.appearing_position;
}

void bt_runner::disappear(const TMsgRunnerDisappear& msg) {
    b_disappear = true;
}

void bt_runner::registerMsgs() {
    DECL_MSG(bt_runner, TMsgRunnerAppear, appear);
    DECL_MSG(bt_runner, TMsgRunnerDisappear, disappear);
}

void bt_runner::load(const json& j, TEntityParseContext& ctx) {
	setEntity(ctx.current_entity);
	attack_distance = j.value("attack_distance", 1.0f);

	waypoint a;
	waypoint b;
	a.position = VEC3(-8.426f, 0.f, 30.011f);
	b.position = VEC3(30.649, 5.f, 7.272f);
	a.type = "floor";
	b.type = "floor";
	a.id = 1;
	b.id = 2;
	a.neighbours.push_back(b.id);
	b.neighbours.push_back(a.id);
	waypoints_map.push_back(a);
	waypoints_map.push_back(b);
	
    create("runner");
}

std::string foo(bool b) {
	if (b)
		return "True";
	else return "False";
}

void bt_runner::debugInMenu() {
	bt::debugInMenu();
	string state;
	if (current == NULL)
		state = "null";
	else state = current->getName();
	ImGui::Text("State %s", state);
	ImGui::Text("b_disappear %s", foo(b_disappear));
	ImGui::Text("b_appear %s", foo(b_appear));
	ImGui::Text("b_chase %s", foo(b_chase));
	ImGui::Text("b_recular %s", foo(b_recular));
	ImGui::Text("on_wall %s", foo(on_wall));
	ImGui::Text("timer %f", debug_timer);

}

void bt_runner::create(string s)
{
    name=s;
    btnode* r = createRoot("runner_p", PRIORITY, NULL, NULL);

    addChild("runner_p", "disappear_s", SEQUENCE, (btcondition)&bt_runner::conditionDisappear, NULL);
    addChild("disappear_s", "stop_1", ACTION, NULL, (btaction)&bt_runner::actionStop);
    addChild("disappear_s", "scream_1", ACTION, NULL, (btaction)&bt_runner::actionScream);
    addChild("disappear_s", "disappear", ACTION, NULL, (btaction)&bt_runner::actionDisappear);

    addChild("runner_p", "chase_p", PRIORITY, (btcondition)&bt_runner::conditionChase, NULL);
    addChild("chase_p", "recular_s", SEQUENCE, (btcondition)&bt_runner::conditionRecular, NULL);
    addChild("recular_s", "recular", ACTION, NULL, (btaction)&bt_runner::actionRecular);
    addChild("recular_s", "recover", ACTION, NULL, (btaction)&bt_runner::actionRecover);

    addChild("chase_p", "attack_p", PRIORITY, (btcondition)&bt_runner::conditionAttack, NULL);
    addChild("attack_p", "attack_wall_r", RANDOM, (btcondition)&bt_runner::conditionAttackWall, NULL);
    addChild("attack_wall_r", "attack_wall_1", ACTION, NULL, (btaction)&bt_runner::actionAttackWall1);
    addChild("attack_wall_r", "attack_wall_2", ACTION, NULL, (btaction)&bt_runner::actionAttackWall2);

    addChild("attack_p", "attack_floor_r", RANDOM, (btcondition)&bt_runner::conditionAttackFloor, NULL);
    addChild("attack_floor_r", "attack_floor_1", ACTION, NULL, (btaction)&bt_runner::actionAttackFloor1);
    addChild("attack_floor_r", "attack_floor_2", ACTION, NULL, (btaction)&bt_runner::actionAttackFloor2);

    addChild("chase_p", "chase", ACTION, NULL, (btaction)&bt_runner::actionChase);

    addChild("runner_p", "appear_s", SEQUENCE, (btcondition)&bt_runner::conditionAppear, NULL);
    addChild("appear_s", "appear", ACTION, NULL, (btaction)&bt_runner::actionAppear);
    //addChild("appear_s", "scream_2", ACTION, NULL, (btaction)&bt_runner::actionScream);

    addChild("runner_p", "hide", ACTION, NULL, (btaction)&bt_runner::actionHide);
}

//----- ACTIONS -----
int bt_runner::actionStop() {
	dbg("stop\n");
    change_color(VEC4(0.5f, 0.0f, 0.0f, 1.0f));
    debug_timer += DT;
    if (debug_timer >= 1.f) {
        debug_timer = 0.f;
        return LEAVE;
    }
    return STAY;
};

int bt_runner::actionScream() {
	dbg("scream\n");
    change_color(VEC4(1.0f, 0.0f, 0.0f, 1.0f));
    debug_timer += DT;
    if (debug_timer >= 1.f) {
        debug_timer = 0.f;
        return LEAVE;
    }
    return STAY;
};

int bt_runner::actionDisappear() {
	dbg("disappear\n");
	EngineTower.disappearEntity("Runner");
    TCompCollider *comp_collider = getMyCollider();
    comp_collider->controller->setPosition(physx::PxExtendedVec3(tower_center.x, tower_center.y, tower_center.z));
    b_disappear = false;
    return LEAVE;
};

int bt_runner::actionRecular() {
	dbg("recular\n");
    change_color(VEC4(0.0f, 0.5f, 0.0f, 1.0f));
    debug_timer += DT;
    if (debug_timer >= 1.f) {
        debug_timer = 0.f;
		b_recular = false;
        return LEAVE;
    }
    return STAY;
};

int bt_runner::actionRecover() {
	dbg("recover\n");
    change_color(VEC4(0.0f, 1.0f, 0.0f, 1.0f));
    debug_timer += DT;
    if (debug_timer >= 1.f) {
        debug_timer = 0.f;
        b_recular = false;
        return LEAVE;
    }
    return STAY;
};

int bt_runner::actionAttackWall1() {
	dbg("attackwall1\n");
    debug_timer += DT;
    if (debug_timer >= 1.f) {
        debug_timer = 0.f;
        killPlayer();
        return LEAVE;
    }
    return STAY;
};

int bt_runner::actionAttackWall2() {
	dbg("attackwall2\n");
    debug_timer += DT;
    if (debug_timer >= 1.f) {
        debug_timer = 0.f;
        killPlayer();
        return LEAVE;
    }
    return STAY;
};

int bt_runner::actionAttackFloor1() {
	dbg("attackfloor1\n");
    change_color(VEC4(0.0f, 0.0f, 1.0f, 1.0f));
    debug_timer += DT;
    if (debug_timer >= 1.f) {
        debug_timer = 0.f;
        killPlayer();
        return LEAVE;
    }
    return STAY;
};

int bt_runner::actionAttackFloor2() {
	dbg("attackfloor2\n");
    change_color(VEC4(0.0f, 0.0f, 0.5f, 1.0f));
    debug_timer += DT;
    if (debug_timer >= 1.f) {
        debug_timer = 0.f;
        killPlayer();
        return LEAVE;
    }
    return STAY;
};

int bt_runner::actionChase() {
	dbg("chase\n");
    change_color(VEC4(0.0f, 0.0f, 0.0f, 1.0f));
	actual_waypoint = waypoints_map[0];
	
	next_waypoint = waypoints_map[1];
	go_to_next_waypoint();


    return LEAVE;
};

int bt_runner::actionAppear() {
	dbg("appear\n");
    TCompCollider* my_collider = getMyCollider();
    my_collider->controller->setPosition(physx::PxExtendedVec3(appearing_position.x, appearing_position.y, appearing_position.z));
	EngineTower.appearEntity("Runner");
	b_appear = false;
	b_chase = true;
    return LEAVE;
};

int bt_runner::actionHide() {
	dbg("hide\n");
	return LEAVE;
};

//----- CONDITIONS -----
bool bt_runner::conditionDisappear() {
    return b_disappear;
};

bool bt_runner::conditionChase() {
    return b_chase;
};

bool bt_runner::conditionRecular() {
    return b_recular;
};

bool bt_runner::conditionAttackWall() {
    return on_wall;
};

bool bt_runner::conditionAttackFloor() {
    return !on_wall;
};

bool bt_runner::conditionAppear() {
    return b_appear;
};

bool bt_runner::conditionAttack() {
	TCompTransform *c_my_transform = getMyTransform();
	VEC3 my_position = c_my_transform->getPosition();

	CEntity *player = (CEntity *)getEntityByName("The Player");
	TCompTransform *c_p_transform = player->get<TCompTransform>();
	VEC3 player_position = c_p_transform->getPosition();

	return (VEC3::Distance(my_position, player_position) < attack_distance);
};

//----- EXTRA -----
void bt_runner::killPlayer() {
    CEntity* player = (CEntity *)getEntityByName("The Player");
    TMsgSetFSMVariable deadMsg;
    deadMsg.variant.setName("hit");
    deadMsg.variant.setBool(true);
    player->sendMsg(deadMsg);
}


int bt_runner::findClosestWaypoint(VEC3 position) {
	float min_dist = INFINITE;
	int closest_waypoint;
	for (auto wp : waypoints_map) {
		float d = VEC3::Distance(position, wp.position);
		if (d < min_dist) {
			min_dist = d;
			closest_waypoint = wp.id;
		}
	}
	return closest_waypoint;
}

void bt_runner::findPath(int origin, int destiny, std::vector<int>& path){


}

void bt_runner::go_to_next_waypoint() {
	if (actual_waypoint.type == "floor") {
		if (next_waypoint.type == "floor") {
			walk();
		}
	}
	else if (actual_waypoint.type == "edge") {
		if (next_waypoint.type == "edge") {
			jump();
		}

	}
	else if (actual_waypoint.type == "wall") {

	}

}

void bt_runner::walk() {
	TCompTransform *c_my_transform = getMyTransform();
	VEC3 myPos = c_my_transform->getPosition();
	dbg("pos: %f %f %f", myPos.x, myPos.y, myPos.z);

	float current_yaw;
	float current_pitch;
	float amount_moved = 0.005f;
	c_my_transform->getYawPitchRoll(&current_yaw, &current_pitch);

	tower_center.y = myPos.y;

	//hardcoded for debug
	bool going_right = true;

	if (!c_my_transform->isInLeft(next_waypoint.position)) {
		current_yaw = going_right ? current_yaw - deg2rad(180) : current_yaw + deg2rad(180);
		float debug = rad2deg(current_yaw);
		going_right = !going_right;
		c_my_transform->setYawPitchRoll(current_yaw, current_pitch);
	}
	else {
		current_yaw = going_right ? current_yaw + 0.1f * amount_moved : current_yaw - 0.1f * amount_moved;
		c_my_transform->setYawPitchRoll(current_yaw, current_pitch);
		VEC3 aux_vector = !going_right ? -1 * c_my_transform->getFront() : c_my_transform->getFront();
		VEC3 newPos = tower_center + (aux_vector * 31.7f);
		c_my_transform->setYawPitchRoll(current_yaw, current_pitch);
		TCompCollider* comp_collider = get<TCompCollider>();
		if (comp_collider && comp_collider->controller)
		{
			VEC3 delta_move = newPos - myPos;
			delta_move.y += -10 * DT;


			PxShape* player_shape;
			comp_collider->controller->getActor()->getShapes(&player_shape, 1);
			PxFilterData filter_data = player_shape->getSimulationFilterData();
			ControllerFilterCallback *filter_controller = new ControllerFilterCallback();
			BasicQueryFilterCallback *query_filter = new BasicQueryFilterCallback();
			PxControllerCollisionFlags flags = comp_collider->controller->move(PxVec3(delta_move.x, delta_move.y, delta_move.z), 0.f, DT, PxControllerFilters(&filter_data, query_filter, filter_controller));
			VEC3 myPos2 = c_my_transform->getPosition();

			if (flags.isSet(physx::PxControllerCollisionFlag::eCOLLISION_SIDES)) {
				current_yaw = going_right ? current_yaw - 0.1f * amount_moved : current_yaw + 0.1f * amount_moved;
				c_my_transform->setYawPitchRoll(current_yaw, current_pitch);
			}
		}
	}
}

void bt_runner::jump() {

}


/*int bt_runner::jumping_state() {
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

			PxShape* player_shape;
			comp_collider->controller->getActor()->getShapes(&player_shape, 1);
			PxFilterData filter_data = player_shape->getSimulationFilterData();
			ControllerFilterCallback *filter_controller = new ControllerFilterCallback();
			BasicQueryFilterCallback *query_filter = new BasicQueryFilterCallback();
			PxControllerCollisionFlags flags = comp_collider->controller->move(PxVec3(delta_move.x, delta_move.y, delta_move.z), 0.f, DT, PxControllerFilters(&filter_data, query_filter, filter_controller));

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
};*/

