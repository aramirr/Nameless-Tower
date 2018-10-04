#include "mcv_platform.h"
#include "bt_runner.h"
#include "entity/entity_parser.h"
#include "components/physics/controller_filter.h"
#include "components/physics/query_filter.h"
#include "skeleton/comp_skeleton.h"

DECL_OBJ_MANAGER("bt_runner", bt_runner);


//Se tiene que añadir el appear directamente en IDLE
void bt_runner::appear(const TMsgRunnerAppear& msg) {
    b_appear = true;
    appearing_position = msg.appearing_position;

    setCurrent(NULL);
}

void bt_runner::disappear(const TMsgRunnerDisappear& msg) {
    b_disappear = true;
    setCurrent(NULL);
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
	attack_distance = j.value("attack_distance", 1.5f);

  auto& j_waypoints = j["waypoints_map"];
  for (size_t i = 0; i < j_waypoints.size(); ++i) {
    load_waypoint(j_waypoints[i]);
  }
  actual_waypoint = 0;

  create("runner");
}


void bt_runner::debugInMenu() {
	bt::debugInMenu();
	std::string state_s;
	if (current == NULL)
		state_s = "null";
	else state_s = current->getName();
	ImGui::Text("State %s", state_s.c_str());
	ImGui::Text("Target %s", target.c_str());
	ImGui::Text("Going Up: %s", going_up? "Si" : "No");
	ImGui::Text("b_disappear: %s", b_disappear ? "Si" : "No");
  ImGui::Text("b_appear: %s", b_appear ? "Si" : "No");
  ImGui::Text("b_chase: %s", b_chase ? "Si" : "No");
  ImGui::Text("b_recular: %s", b_recular ? "Si" : "No");
	ImGui::Text("timer %f", debug_timer);
	if (actual_waypoint >= 0 && path.size() > 0) {
		ImGui::Text("actual_waypoint type %s", waypoints_map[path[actual_waypoint]].type.c_str());
		ImGui::Text("actual_waypoint %i", path[actual_waypoint]);
	}
  anim_debug_changed = ImGui::DragInt("ANIM", &anim_id, 0.1f, 0, 8);

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

    addChild("chase_p", "attack", ACTION, (btcondition)&bt_runner::conditionAttack, (btaction)&bt_runner::actionAttack);

    addChild("chase_p", "chase", ACTION, NULL, (btaction)&bt_runner::actionChase);

    addChild("runner_p", "appear_s", SEQUENCE, (btcondition)&bt_runner::conditionAppear, NULL);
    addChild("appear_s", "appear", ACTION, NULL, (btaction)&bt_runner::actionAppear);
    //addChild("appear_s", "appear_pose", ACTION, NULL, (btaction)&bt_runner::actionAppearPose);
    //addChild("appear_s", "scream2", ACTION, NULL, (btaction)&bt_runner::actionScream);

    addChild("runner_p", "hide", ACTION, NULL, (btaction)&bt_runner::actionHide);
}

//----- ACTIONS -----
int bt_runner::actionStop() {
  debug_timer += DT;
  addGravity();
  if (debug_timer >= 1.f) {
      debug_timer = 0.f;
      return LEAVE;
  }
  return STAY;
};

int bt_runner::actionScream() {
  if (anim_state != "scream") {
    anim_state = "scream";
    //change_animation(ERunnerAnimations::RunnerIdle, false, 0.f, 0.f, true);
    change_animation(ERunnerAnimations::RunnerScreamShort, true, 0.f, 0.f, true);
  }
  addGravity();
  debug_timer += DT;
  if (debug_timer >= 2.0f) {
      debug_timer = 0.f;
      return LEAVE;
  }
  return STAY;
};

int bt_runner::actionDisappear() {
	EngineTower.disappearEntity("Runner");
	TCompTransform* my_transform = getMyTransform();
	my_transform->setPosition(VEC3::Zero);
  TCompCollider *comp_collider = getMyCollider();
  comp_collider->controller->setPosition(physx::PxExtendedVec3(tower_center.x, tower_center.y, tower_center.z)); 
	b_disappear = false;
	b_chase = false;
  return LEAVE;
};

int bt_runner::actionRecular() {
    addGravity();
    debug_timer += DT;
    if (debug_timer >= 1.f) {
        debug_timer = 0.f;
		b_recular = false;
        return LEAVE;
    }
    return STAY;
};

int bt_runner::actionRecover() {
    addGravity();
    debug_timer += DT;
    if (debug_timer >= 1.f) {
        debug_timer = 0.f;
        b_recular = false;
        return LEAVE;
    }
    return STAY;
};

int bt_runner::actionAttack() {
		if (anim_state != "attack") {
			anim_state = "attack";
			//change_animation(ERunnerAnimations::RunnerIdle, false, 0.f, 0.f, true);
			change_animation(ERunnerAnimations::RunnerAttack, true, 0.f, 0.f, true);
		}

    addGravity();

    debug_timer += DT;
    if (debug_timer >= 0.4f) {
        debug_timer = 0.f;
				b_disappear = true;
        killPlayer();
        return LEAVE;
    }
    return STAY;
};

int bt_runner::actionChase() {
	recalculate_timer += DT;
	if (recalculate_timer > 0.2f) {
		recalculate_timer = 0.f;
		recalculate_path();
	}
	//dbg("target %s", target.c_str());
  TCompTransform *c_my_transform = getMyTransform();
  VEC3 my_position = c_my_transform->getPosition();

  CEntity *player = (CEntity *)getEntityByName("The Player");
  TCompTransform *c_p_transform = player->get<TCompTransform>();
  VEC3 player_position = c_p_transform->getPosition();

  float distance_to_player = VEC3::Distance(my_position, player_position);
	float distance_to_waypoint;
	if (target == "waypoint") {
		if (next_waypoint >= 0) distance_to_waypoint = VEC3::Distance(my_position, waypoints_map[path[next_waypoint]].position);
		else distance_to_waypoint = VEC3::Distance(my_position, waypoints_map[path[actual_waypoint]].position);
	}
	else {
		distance_to_waypoint = VEC3::Distance(my_position, waypoints_map[second_closest_waypoint].position);
	}

  /*if (on_jump) chase_waypoint();
	else if (distance_to_waypoint < distance_to_player) chase_waypoint();
	else chase_player();*/
	chase_waypoint();

  /*if (conditionAttack()) {
    return LEAVE;
  }*/

	//Cambiar a STAY y poner interrupciones que reinicien el BT
    return STAY;
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
	going_right = false;
	going_up = true;
	on_jump = false;
  recalculate_timer = 0.f;
	recalculate_path();

  return LEAVE;
};

int bt_runner::actionAppearPose() {
  if (anim_state != "appear_pose") {
    anim_state = "appear_pose";
    change_animation(ERunnerAnimations::RunnerIdle, false, 0.f, 0.f, true);
    change_animation(ERunnerAnimations::RunnerAparece, true, 0.f, 0.f, true);
  }
  debug_timer += DT;
  if (debug_timer >= 4.4f) {
    debug_timer = 0.f;
    return LEAVE;
  }
  return STAY;
};

int bt_runner::actionHide() {

	//dbg("hide\n");
	return STAY;
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

void bt_runner::addGravity() {
  TCompCollider* comp_collider = get<TCompCollider>();
  TCompTransform* comp_transform = get<TCompTransform>();
  PxRigidActor* rigidActor = ((PxRigidActor*)comp_collider->actor);
  VEC3 delta_move = comp_transform->getPosition();
  delta_move.y += -1.f * DT;
	float d_y = -10.f * DT;
  PxShape* player_shape;
  comp_collider->controller->getActor()->getShapes(&player_shape, 1);
  PxFilterData filter_data = player_shape->getSimulationFilterData();
  ControllerFilterCallback *filter_controller = new ControllerFilterCallback();
  BasicQueryFilterCallback *query_filter = new BasicQueryFilterCallback();
  PxControllerCollisionFlags flags = comp_collider->controller->move(PxVec3(0.f, d_y, 0.f), 0.f, DT, PxControllerFilters(&filter_data, query_filter, filter_controller));//*/
}


int bt_runner::findClosestWaypoint(VEC3 position) {
	float min_dist = INFINITE;
	int closest_waypoint;
  int xx = 0;
	for (auto wp : waypoints_map) {
    ++xx;
		float d = VEC3::Distance(position, wp.position);
		if (d < min_dist) {
			min_dist = d;
			closest_waypoint = wp.id;
		}
	}
  int a = 0;
	return closest_waypoint;
}

int bt_runner::findSecondClosestWaypoint(VEC3 position, int closest_waypoint_id) {
	float min_dist = INFINITE;
	int closest_waypoint;
	for (auto wp : waypoints_map) {
		if (wp.id != closest_waypoint_id) {
			float d = VEC3::Distance(position, wp.position);
			if (d < min_dist) {
				min_dist = d;
				closest_waypoint = wp.id;
			}
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
    //u = p[u];
    if (u < 0 || u > p.size()) {
      int a = 0;
      TCompTransform* my_transform = getMyTransform();
      VEC3 pos = my_transform->getPosition();
      CEntity* e_player = (CEntity*)getEntityByName("The Player");
      TCompTransform* player_transform = e_player->get<TCompTransform>();
      VEC3 p_pos = player_transform->getPosition();
      a = 1;
    }
		path.push_back(p[u]);
		u = p[u];
	}
}

void bt_runner::chase_player() {
	dbg(" -- c_p\n");
	CEntity* e_player = (CEntity*)getEntityByName("The Player");
	TCompTransform* player_transform = e_player->get<TCompTransform>();

	second_closest_waypoint = findSecondClosestWaypoint(player_transform->getPosition(), actual_waypoint);

	if (waypoints_map[path[actual_waypoint]].type == "edge") {
		//next_waypoint = findSecondClosestWaypoint(player_transform->getPosition(), actual_waypoint);
		if (waypoints_map[second_closest_waypoint].type == "edge") {
			target = "player";
			jump();
		}
		else {
			if (anim_state != "chase_player") {
				anim_state = "chase_player";
				change_animation(ERunnerAnimations::RunnerRunCerca, false, 0.f, 0.f, true);
			}
			target = "player";
			walk();
		}

	}
	else {
		if (anim_state != "chase_player") {
			anim_state = "chase_player";
			change_animation(ERunnerAnimations::RunnerRunCerca, false, 0.f, 0.f, true);
		}
		target = "player";
		walk();
	}
	
  
}

void bt_runner::chase_waypoint() {
	//dbg(" -- c_w\n");
	if (next_waypoint >= 0 || target == "player") {
		if (waypoints_map[path[actual_waypoint]].type == "floor") {
			target = "waypoint";
			walk();
		}
		else if (waypoints_map[path[actual_waypoint]].type == "edge") {
			if (target == "waypoint" && waypoints_map[path[next_waypoint]].type == "edge") {
				target = "waypoint";
				jump();
			}
			else if (target == "player" && waypoints_map[second_closest_waypoint].type == "edge") {
				jump();
			}
			else if (target == "waypoint"){
				walk();
			}
		}
	}

	TCompTransform* my_transform = getMyTransform();
	if (target == "waypoint" && (next_waypoint < 0 || VEC3::Distance(my_transform->getPosition(), waypoints_map[path[next_waypoint]].position) <= 1.0f)) {
		dbg("--------------------- aw: %i - nw: %i - d: %f\n", path[actual_waypoint], path[next_waypoint], VEC3::Distance(my_transform->getPosition(), waypoints_map[path[next_waypoint]].position));

		on_jump = false;
		going_up = true;
		actual_waypoint = next_waypoint;
		--next_waypoint;
		if (next_waypoint < 0) {
			recalculate_path();
		}
	}
}

void bt_runner::walk() {
	VEC3 target_position;
	if (target == "player") {
		if (anim_state != "chase_player") {
			anim_state = "chase_player";
			change_animation(ERunnerAnimations::RunnerRunCerca, false, 0.f, 0.f, true);
		}
		CEntity* e_player = (CEntity*)getEntityByName("The Player");
		TCompTransform* player_transform = e_player->get<TCompTransform>();
		target_position = player_transform->getPosition();
	}
	else {
		if (anim_state != "walk") {
			anim_state = "walk";
			change_animation(ERunnerAnimations::RunnerRun, false, 0.f, 0.f, true);
		}
		if (next_waypoint >= 0) target_position = waypoints_map[path[next_waypoint]].position; 
		else target_position = waypoints_map[path[actual_waypoint]].position;
	}

	TCompTransform *c_my_transform = getMyTransform();
	VEC3 myPos = c_my_transform->getPosition();

	float current_yaw;
	float current_pitch;
	float amount_moved = speed * DT;
	c_my_transform->getYawPitchRoll(&current_yaw, &current_pitch);

	tower_center.y = myPos.y;

  TCompCollider* comp_collider = get<TCompCollider>();
	if (!c_my_transform->isInFront(target_position)) {
		/*if (anim_state != "giro") {
			anim_state = "giro";
			change_animation(ERunnerAnimations::RunnerGiro, true, 0.f, 0.f, true);
		}*/
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
		if (target == "player") {
			if (anim_state != "chase_player") {
				anim_state = "chase_player";
				change_animation(ERunnerAnimations::RunnerRunCerca, false, 0.f, 0.f, true);
			}
		}
		else if (anim_state != "walk") {
			anim_state = "walk";
			change_animation(ERunnerAnimations::RunnerRun, false, 0.f, 0.f, true);
		}
	
		current_yaw = going_right ? current_yaw + 0.1f * amount_moved : current_yaw - 0.1f * amount_moved;
		c_my_transform->setYawPitchRoll(current_yaw, current_pitch);
		VEC3 aux_vector = going_right ? -1 * c_my_transform->getLeft() : c_my_transform->getLeft();
		VEC3 newPos = tower_center + (aux_vector * EngineTower.getTowerRadius());
		c_my_transform->setYawPitchRoll(current_yaw, current_pitch);
		if (comp_collider && comp_collider->controller)
		{
			VEC3 delta_move = newPos - myPos;
			delta_move.y += -10.f * DT;

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
		}
	}
}

void bt_runner::jump() {
  VEC3 target_position;
  if (target == "player") {
    target_position = waypoints_map[second_closest_waypoint].position;

  }
  else {
    target_position = waypoints_map[path[next_waypoint]].position;
  }

	TCompTransform *c_my_transform = getMyTransform();
	VEC3 myPos = c_my_transform->getPosition();
	float gravity = 15.f;
	float Yc = -1.f;
	if (!on_jump) {

		// C position is the point with maxHeight and Vy = 0
		
		if (waypoints_map[path[actual_waypoint]].position.y <= target_position.y + 0.5f)
			Yc = target_position.y + 0.5f;
		else
			Yc = waypoints_map[path[actual_waypoint]].position.y + 0.5f;


		//dbg("wp: %i - Yc: %f - ypos: %f\n", path[actual_waypoint], Yc, myPos.y);

		float TimeBC = 2 * (target_position.y - Yc) / -gravity;
		float TimeAC = TimeBC;
		float TimeAB = TimeAC + TimeBC;

		Vx = distance_arc(waypoints_map[path[actual_waypoint]].position, target_position) / TimeAB; // Radians/seconds
		Vy = gravity * TimeAC;
	}
	dbg("wp: %i - nwp: %i - Yc: %f - ypos: %f\n", path[actual_waypoint], path[next_waypoint], Yc, myPos.y);
	
	float current_yaw;
	float current_pitch;
	c_my_transform->getYawPitchRoll(&current_yaw, &current_pitch);

	tower_center.y = myPos.y;

	TCompCollider* comp_collider = get<TCompCollider>();
	if (!c_my_transform->isInFront(target_position) && target_position.x != myPos.x && target_position.z != myPos.z && !on_jump) {
		/*if (anim_state != "giro") {
			anim_state = "giro";
			change_animation(ERunnerAnimations::RunnerGiro, true, 0.f, 0.f, true);
		}*/
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
		if (anim_state != "jump_up") {
			anim_state = "jump_up";
			change_animation(ERunnerAnimations::RunnerJumpLoop, false, 0.1f, 0.f, true);
			if (waypoints_map[path[actual_waypoint]].position.y > target_position.y + 2.0f)
				change_animation(ERunnerAnimations::RunnerJumpShort, true, 0.1f, 0.f, true);
			else change_animation(ERunnerAnimations::RunnerJumpUp, true, 0.1f, 0.f, true);
		}

		
		current_yaw = going_right ? current_yaw + Vx*DT : current_yaw - Vx * DT;
		c_my_transform->setYawPitchRoll(current_yaw, current_pitch);
		VEC3 aux_vector = going_right ? -1 * c_my_transform->getLeft() : c_my_transform->getLeft();
		VEC3 newPos = tower_center + (aux_vector * EngineTower.getTowerRadius());
		c_my_transform->setYawPitchRoll(current_yaw, current_pitch);

		if (comp_collider && comp_collider->controller)
		{
			VEC3 delta_move;
			delta_move = newPos - myPos;

			delta_move.y += Vy*DT;
			Vy -= gravity * DT;

			PxShape* player_shape;
			comp_collider->controller->getActor()->getShapes(&player_shape, 1);
			PxFilterData filter_data = player_shape->getSimulationFilterData();
			ControllerFilterCallback *filter_controller = new ControllerFilterCallback();
			BasicQueryFilterCallback *query_filter = new BasicQueryFilterCallback();
			PxControllerCollisionFlags flags = comp_collider->controller->move(PxVec3(delta_move.x, delta_move.y, delta_move.z), 0.f, DT, PxControllerFilters(&filter_data, query_filter, filter_controller));
			VEC3 myPos2 = c_my_transform->getPosition();

			if (flags.isSet(physx::PxControllerCollisionFlag::eCOLLISION_SIDES)) {
				current_yaw = going_right ? current_yaw - Vx * DT : current_yaw + Vx * DT;
				c_my_transform->setYawPitchRoll(current_yaw, current_pitch);
			}
      if (!going_up && flags.isSet(physx::PxControllerCollisionFlag::eCOLLISION_DOWN)) {
				going_up = true;
        if (anim_state != "jump_land") {
          anim_state = "jump_land";
          change_animation(ERunnerAnimations::RunnerJumpLand, true, 0.1f, 0.f, true);
        }
      }
		}
	}
	on_jump = true;
}

float bt_runner::distance_arc(VEC3 pos1, VEC3 pos2) {
	float alpha = asin(pos1.z / EngineTower.getTowerRadius());
	float beta = asin(pos2.z / EngineTower.getTowerRadius());
	
	float dist;
	if (alpha > beta) dist = alpha - beta;
	else dist = beta - alpha;

	return dist;
}

void bt_runner::calculate_top_jump_position(VEC3 target_position) {
	float alpha = asin(waypoints_map[path[actual_waypoint]].position.z / EngineTower.getTowerRadius());
	float beta = asin(target_position.z / EngineTower.getTowerRadius());
  float charlie = (alpha + beta) / 2.f;
	//charlie = ((beta - alpha) / 4.f) + alpha;

	float top_y;
	if (waypoints_map[path[actual_waypoint]].position.y <= target_position.y)
		top_y = target_position.y + 1.5f;
	else
		top_y = waypoints_map[path[actual_waypoint]].position.y + 0.5f;

	top_jump_position = VEC3(EngineTower.getTowerRadius()*cos(charlie), top_y, EngineTower.getTowerRadius()*sin(charlie));

  if (not ((top_jump_position.x > waypoints_map[path[actual_waypoint]].position.x and top_jump_position.x < target_position.x) or
    (top_jump_position.x < waypoints_map[path[actual_waypoint]].position.x and top_jump_position.x > target_position.x))) {
    top_jump_position.x = -top_jump_position.x;
  }

  if (not ((top_jump_position.z > waypoints_map[path[actual_waypoint]].position.z and top_jump_position.z < target_position.z) or
    (top_jump_position.z < waypoints_map[path[actual_waypoint]].position.z and top_jump_position.z > target_position.z))) {
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
	int target_pos = findClosestWaypoint(c_p_transform->getPosition());

	findPath(origin, 60);
	actual_waypoint = path.size() -1;
	next_waypoint = actual_waypoint -1;
	target == "waypoint";
}

// Animation functions
void bt_runner::change_animation(int animation_id, bool is_action, float in_delay, float out_delay, bool clear = true) {
  CEntity* e = h_entity;
  TCompSkeleton* skeleton = e->get<TCompSkeleton>();
  assert(skeleton);

  //skeleton->playAnimation(animation_id, is_action, in_delay, out_delay, clear);
}

void bt_runner::remove_animation(int animation_id) {
  CEntity* e = h_entity;
  TCompSkeleton* skeleton = e->get<TCompSkeleton>();
  assert(skeleton);

  skeleton->removeAnimation(animation_id);
}

void bt_runner::clear_animations(float out_delay) {
  CEntity* e = h_entity;
  TCompSkeleton* skeleton = e->get<TCompSkeleton>();
  assert(skeleton);

  skeleton->clearActions(out_delay);
}