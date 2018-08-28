#include "mcv_platform.h"
#include "bt_runner.h"
#include "entity/entity_parser.h"
#include "components/physics/controller_filter.h"
#include "components/physics/query_filter.h"
#include "skeleton/comp_skeleton.h"

DECL_OBJ_MANAGER("bt_runner", bt_runner);


//Se tiene que a�adir el appear directamente en IDLE
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

void bt_runner::load_waypoint(const json& j) {
  waypoint w;
  w.position = loadVEC3(j["pos"]);
  std::string aux_type = j["type"];
  w.type = aux_type;
  w.id = int(j["id"]);
  auto& j_neighbours = j["neighbours"];
  for (auto it = j_neighbours.begin(); it != j_neighbours.end(); ++it) {
    std::string id = it.value().get< std::string >();
	Warc a;
	a.first = 1.0f;
	a.second = atoi(id.c_str());
    w.neighbours.push_back(a);
  }

  waypoints_map.push_back(w);

}

void bt_runner::load(const json& j, TEntityParseContext& ctx) {
	setEntity(ctx.current_entity);
	attack_distance = j.value("attack_distance", 1.0f);

  auto& j_waypoints = j["waypoints_map"];
  for (size_t i = 0; i < j_waypoints.size(); ++i) {
    load_waypoint(j_waypoints[i]);
  }
  actual_waypoint = 0;

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
	//dbg("stop\n");
    change_color(VEC4(0.5f, 0.0f, 0.0f, 1.0f));
    debug_timer += DT;
    if (debug_timer >= 1.f) {
        debug_timer = 0.f;
        return LEAVE;
    }
    return STAY;
};

int bt_runner::actionScream() {
	//dbg("scream\n");
    change_color(VEC4(1.0f, 0.0f, 0.0f, 1.0f));
    debug_timer += DT;
    if (debug_timer >= 1.f) {
        debug_timer = 0.f;
        return LEAVE;
    }
    return STAY;
};

int bt_runner::actionDisappear() {
	//dbg("disappear\n");
	EngineTower.disappearEntity("Runner");
    TCompCollider *comp_collider = getMyCollider();
    comp_collider->controller->setPosition(physx::PxExtendedVec3(tower_center.x, tower_center.y, tower_center.z));
    b_disappear = false;
    return LEAVE;
};

int bt_runner::actionRecular() {
	//dbg("recular\n");
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
	//dbg("recover\n");
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
	//dbg("attackwall1\n");
    debug_timer += DT;
    if (debug_timer >= 1.f) {
        debug_timer = 0.f;
        killPlayer();
        return LEAVE;
    }
    return STAY;
};

int bt_runner::actionAttackWall2() {
	//dbg("attackwall2\n");
    debug_timer += DT;
    if (debug_timer >= 1.f) {
        debug_timer = 0.f;
        killPlayer();
        return LEAVE;
    }
    return STAY;
};

int bt_runner::actionAttackFloor1() {
	//dbg("attackfloor1\n");
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
	//dbg("attackfloor2\n");
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
	
	//getPath();
	if (b_chase_player)
		chase_player();
	else chase_waypoint();

	//Cambiar a STAY y poner interrupciones que reinicien el BT
    return LEAVE;
};

int bt_runner::actionAppear() {
	//dbg("appear\n");
  TCompCollider* my_collider = getMyCollider();
  my_collider->controller->setPosition(physx::PxExtendedVec3(appearing_position.x, appearing_position.y, appearing_position.z));
  TCompTransform* my_transform = getMyTransform();
  tower_center.y = appearing_position.y;
  my_transform->lookAt(appearing_position, tower_center);
  float y, p;
  my_transform->getYawPitchRoll(&y, &p);
  y += deg2rad(90);
  my_transform->setYawPitchRoll(y, p);

	EngineTower.appearEntity("Runner");
	b_appear = false;
	b_chase = true;
  CEntity* e = h_entity;
  TCompSkeleton* skeleton = e->get<TCompSkeleton>();
  assert(skeleton);

  skeleton->playAnimation(0, false, 0.f, 0.f, true);
	recalculate_path();

    return LEAVE;
};

int bt_runner::actionHide() {

	//dbg("hide\n");
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

void bt_runner::calculate_distances_graph() {
	float perimeter = 2 * 3.1415 * EngineTower.getTowerRadius();
	for (waypoint &w : waypoints_map) {
		float alpha = asin(w.position.z / EngineTower.getTowerRadius());

		for (Warc &w2 : w.neighbours) {
			float beta = asin(waypoints_map[w2.second].position.z / EngineTower.getTowerRadius());
			float charlie = abs(alpha - beta);

			float dist = (rad2deg(charlie)*perimeter) / 360;
			dist += abs(w.position.y - waypoints_map[w2.second].position.y);
			w2.first = dist;
		}
	}
}

void bt_runner::findPath(int origin, int destiny){
	calculate_distances_graph();
	int n = waypoints_map.size();
	vector<float> d (n, INFINITE);
	d[origin] = 0;
	vector<int> p (n, -1);
	vector<bool> visited(n, false);
	std::priority_queue<Warc, vector<Warc>, greater<Warc>> Q;
	Q.push(Warc(0, origin));

	while (not Q.empty()) {
		int u = Q.top().second;
		Q.pop();
		
		if (not visited[u]) {
			visited[u] = true;
			for (Warc a : waypoints_map[u].neighbours) {
				int v = a.second;
				float c = a.first;

				if (d[v] > d[u] + c) {
					d[v] = d[u] + c;
					p[v] = u;
					Q.push(Warc(d[v], v));
				}
			}
		}
	}

	while (path.size() > 0) {
		path.pop_back();
	}

	int u = destiny;
	path.push_back(u);
	while (u != origin) {
		path.push_back(p[u]);
		u = p[u];
	}
}

void bt_runner::chase_player() {
  
}

void bt_runner::chase_waypoint() {
	if (waypoints_map[path[actual_waypoint]].type == "floor") {
		walk();
	}
	else if (waypoints_map[path[actual_waypoint]].type == "edge") {
		if (waypoints_map[path[next_waypoint]].type == "edge") {
			jump();
		}
		else walk();
	}
    TCompTransform* my_transform = getMyTransform();
    if (VEC3::Distance(my_transform->getPosition(), waypoints_map[path[next_waypoint]].position) <= 1.0f) {
	  going_up = true;
      actual_waypoint = next_waypoint;
	  --next_waypoint;
	  if (next_waypoint < 0) {
		  recalculate_path();
	  }

      //get_next_waypoint();

	  //findPath(0, 7);
    }
}

void bt_runner::walk() {
	TCompTransform *c_my_transform = getMyTransform();
	VEC3 myPos = c_my_transform->getPosition();
	//dbg("pos: %f %f %f", myPos.x, myPos.y, myPos.z);

	float current_yaw;
	float current_pitch;
	float amount_moved = speed * DT;
	c_my_transform->getYawPitchRoll(&current_yaw, &current_pitch);

	tower_center.y = myPos.y;

  TCompCollider* comp_collider = get<TCompCollider>();
	if (!c_my_transform->isInFront(waypoints_map[path[next_waypoint]].position)) {
		current_yaw = going_right ? current_yaw - deg2rad(180) : current_yaw + deg2rad(180);
		float debug = rad2deg(current_yaw);
		going_right = !going_right;

		PxRigidActor* rigidActor = ((PxRigidActor*)comp_collider->actor);
		PxTransform tr = rigidActor->getGlobalPose();
		auto& rot2 = c_my_transform->getRotation();


		c_my_transform->setYawPitchRoll(current_yaw, current_pitch);

   
		auto& rot = c_my_transform->getRotation();
		tr.q = PxQuat(rot.x, rot.y, rot.z, rot.w);
		rigidActor->setGlobalPose(tr);

	}
	else {
		current_yaw = going_right ? current_yaw + 0.1f * amount_moved : current_yaw - 0.1f * amount_moved;
		c_my_transform->setYawPitchRoll(current_yaw, current_pitch);
		VEC3 aux_vector = going_right ? -1 * c_my_transform->getLeft() : c_my_transform->getLeft();
		VEC3 newPos = tower_center + (aux_vector * EngineTower.getTowerRadius());
		c_my_transform->setYawPitchRoll(current_yaw, current_pitch);
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
	calculate_top_jump_position();

	TCompTransform *c_my_transform = getMyTransform();
	VEC3 myPos = c_my_transform->getPosition();

	float current_yaw;
	float current_pitch;
	float amount_moved = speed * DT * 1.5f;
	c_my_transform->getYawPitchRoll(&current_yaw, &current_pitch);

	tower_center.y = myPos.y;

	TCompCollider* comp_collider = get<TCompCollider>();
	if (!c_my_transform->isInFront(waypoints_map[path[next_waypoint]].position)) {
		current_yaw = going_right ? current_yaw - deg2rad(180) : current_yaw + deg2rad(180);
		float debug = rad2deg(current_yaw);
		going_right = !going_right;

		PxRigidActor* rigidActor = ((PxRigidActor*)comp_collider->actor);
		PxTransform tr = rigidActor->getGlobalPose();

		c_my_transform->setYawPitchRoll(current_yaw, current_pitch);
		auto& rot = c_my_transform->getRotation();
		tr.q = PxQuat(rot.x, rot.y, rot.z, rot.w);
		rigidActor->setGlobalPose(tr);
	}
	else {
		current_yaw = going_right ? current_yaw + 0.1f * amount_moved : current_yaw - 0.1f * amount_moved;
		c_my_transform->setYawPitchRoll(current_yaw, current_pitch);
		VEC3 aux_vector = going_right ? -1 * c_my_transform->getLeft() : c_my_transform->getLeft();
		VEC3 newPos = tower_center + (aux_vector * EngineTower.getTowerRadius());
		c_my_transform->setYawPitchRoll(current_yaw, current_pitch);
		if (comp_collider && comp_collider->controller)
		{

			if (going_up) {
				float d1 = distance_x_z(newPos, top_jump_position);

				float d2 = distance_x_z(waypoints_map[path[actual_waypoint]].position, top_jump_position);
				float perc = 1 - (d1 / d2);
				if (perc < 0) perc = 0;
				if (perc >= 0.99) {
					going_up = false;
				}
				//perc = perc * perc*perc * (perc * (6.f*perc - 15.f) + 10.f);
				perc = sin(perc * 3.1415 * 0.5f);

				float aux = lerp(waypoints_map[path[actual_waypoint]].position.y, top_jump_position.y, perc);
				newPos.y = aux;
			}
			else {
				float d1 = distance_x_z(newPos, waypoints_map[path[next_waypoint]].position);
				float d2 = distance_x_z(top_jump_position, waypoints_map[path[next_waypoint]].position);
				float perc = 1 - (d1 / d2);
				if (perc < 0) perc = 0;
				//perc = perc * perc*perc * (perc * (6.f*perc - 15.f) + 10.f);
				perc = sin(perc * 3.1415 * 0.5f);

				float aux = lerp(top_jump_position.y, waypoints_map[path[next_waypoint]].position.y, perc);
				newPos.y = aux;

			}
			VEC3 delta_move = newPos - myPos;

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

void bt_runner::calculate_top_jump_position() {
	float alpha = asin(waypoints_map[path[actual_waypoint]].position.z / EngineTower.getTowerRadius());
	float beta = asin(waypoints_map[path[next_waypoint]].position.z / EngineTower.getTowerRadius());
  	float charlie = (alpha + beta) / 2.f;
	float top_y = max(waypoints_map[path[actual_waypoint]].position.y, waypoints_map[path[next_waypoint]].position.y) + 2.0f;

	top_jump_position = VEC3(EngineTower.getTowerRadius()*cos(charlie), top_y, EngineTower.getTowerRadius()*sin(charlie));

  if (not ((top_jump_position.x > waypoints_map[path[actual_waypoint]].position.x and top_jump_position.x < waypoints_map[path[next_waypoint]].position.x) or
    (top_jump_position.x < waypoints_map[path[actual_waypoint]].position.x and top_jump_position.x > waypoints_map[path[next_waypoint]].position.x))) {
    top_jump_position.x = -top_jump_position.x;
  }

  if (not ((top_jump_position.z > waypoints_map[path[actual_waypoint]].position.z and top_jump_position.z < waypoints_map[path[next_waypoint]].position.z) or
    (top_jump_position.z < waypoints_map[path[actual_waypoint]].position.z and top_jump_position.z > waypoints_map[path[next_waypoint]].position.z))) {
    top_jump_position.z = -top_jump_position.z;
  }


/*
  
  // Cosinus positivo entre 270-90 grados. Negativo entre 90-270 grados.
  if (charlie < deg2rad(90) and charlie > deg2rad(270) and top_jump_position.z < 0)
    top_jump_position.z = -top_jump_position.z;

  // Cosinus positivo entre 270-90 grados. Negativo entre 90-270 grados.
  if (charlie > deg2rad(0) and charlie < deg2rad(180) and top_jump_position.x < 0)
    top_jump_position.x = -top_jump_position.x;*/
}

float bt_runner::distance_x_z(VEC3 v1, VEC3 v2) {
	v1.y = 0.f;
	v2.y = 0.f;
	return VEC3::Distance(v1, v2);
}


void bt_runner::recalculate_path() {
	TCompTransform* my_transform = getMyTransform();
	int origin = findClosestWaypoint(my_transform->getPosition());

	CEntity *player = (CEntity *)getEntityByName("The Player");
	TCompTransform *c_p_transform = player->get<TCompTransform>();
	int target = findClosestWaypoint(c_p_transform->getPosition());

	findPath(origin, target);
	actual_waypoint = path.size() -1;
	next_waypoint = actual_waypoint -1;
	if (path.size() < 2) b_chase_player = true;
	else b_chase_player = false;
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

