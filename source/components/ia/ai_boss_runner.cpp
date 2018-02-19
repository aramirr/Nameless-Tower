#include "mcv_platform.h"
#include "handle/handle.h"
#include "ai_boss_runner.h"
#include "entity/entity_parser.h"
#include "components/comp_transform.h"
#include "render/render_utils.h"

DECL_OBJ_MANAGER("ai_boss_runner", CAIBossRunner);

void CAIBossRunner::Init()
{
	// insert all states in the map
	AddState("resetwpts", (statehandler)&CAIBossRunner::ResetWptsState);
	AddState("nextwpt", (statehandler)&CAIBossRunner::NextWptState);
	AddState("hide", (statehandler)&CAIBossRunner::HideState);
	AddState("appear", (statehandler)&CAIBossRunner::AppearState);
	AddState("disappear", (statehandler)&CAIBossRunner::DissapearState);
	AddState("chase", (statehandler)&CAIBossRunner::ChaseState);
	AddState("attack", (statehandler)&CAIBossRunner::AttackState);
	AddState("die", (statehandler)&CAIBossRunner::DieState);

	// reset the state
	ChangeState("resetwpts");
	currentWaypoint = 0;
}

void CAIBossRunner::debugInMenu() {
	IAIController::debugInMenu();
	ImGui::Text("Distance player %f", distance_to_player);
	ImGui::Text("aux_count  %f", aux_count);
	if (ImGui::TreeNode("Waypoints")) {
		for (auto& v : _waypoints) {
			ImGui::PushID(&v);
			ImGui::DragFloat3("Point", &v.x, 0.1f, -20.f, 20.f);
			ImGui::PopID();
		}
		ImGui::TreePop();
	}
}

void CAIBossRunner::load(const json& j, TEntityParseContext& ctx) {
	setEntity(ctx.current_entity);
	Init();
	chase_distance = j.value("chase_distance", 3.0f);
	attack_distance = j.value("attack_distance", 1.0f);
	speed_factor = j.value("speed_factor", 5.0f);
	tower_radius = j.value("tower_radius", 15.0f);
	auto& j_waypoints = j["waypoints"];
	for (auto it = j_waypoints.begin(); it != j_waypoints.end(); ++it) {
		VEC3 p = loadVEC3(it.value());
		addWaypoint(p);
	}
}

void CAIBossRunner::ResetWptsState() {
	currentWaypoint = 0;
	TCompTransform *my_pos = getMyTransform();
	my_pos->lookAt(_waypoints[currentWaypoint], tower_center);
	float y, p, r;
	my_pos->getYawPitchRoll(&y, &p, &r);
	y -= deg2rad(90);
	my_pos->setYawPitchRoll(y, p, r);
	if (my_pos->isInLeft(tower_center))
		going_right = true;
	else going_right = false;
	change_color(VEC4(1, 1, 0, 1));
	//my_render->is_active = false;
	ChangeState("hide");
}

void CAIBossRunner::NextWptState() {
	++currentWaypoint;
	TCompTransform *my_pos = getMyTransform();
	if (currentWaypoint < _waypoints.size()) {
		my_pos->lookAt(_waypoints[currentWaypoint], tower_center);
		float y, p, r;
		my_pos->getYawPitchRoll(&y, &p, &r);
		y += deg2rad(90);
		my_pos->setYawPitchRoll(y, p, r);
		if (my_pos->isInLeft(tower_center))
			going_right = true;
		else going_right = false;
		TCompRender *my_render = getMyRender();
		my_render->color = VEC4(1, 1, 0, 1);
		//my_render->is_active = false;
		ChangeState("hide");
	}
	else {
		ChangeState("die");
	}
}

void CAIBossRunner::HideState() {
	CEntity *player = (CEntity *)getEntityByName("The Player");
	TCompTransform *my_pos = getMyTransform();
	TCompTransform *ppos = player->get<TCompTransform>();
	VEC3 a = my_pos->getPosition();
	VEC3 b = ppos->getPosition();
	distance_to_player = VEC3::Distance(a, b);
	if (distance_to_player < chase_distance)
		ChangeState("appear");
}

void CAIBossRunner::AppearState() {
	TCompTransform *c_my_transform = getMyTransform();
	going_right = c_my_transform->isInLeft(tower_center);
	TCompRender *my_render = getMyRender();
	my_render->is_active = true;
 	ChangeState("chase");
}

void CAIBossRunner::ChaseState(float dt) {
	TCompTransform *c_my_transform = getMyTransform();
	VEC3 myPos = c_my_transform->getPosition();
	CEntity *player = (CEntity *)getEntityByName("The Player");
	TCompTransform *c_p_transform = player->get<TCompTransform>();
	VEC3 ppos = c_p_transform->getPosition();

	
	float current_yaw;
	float current_pitch;
	float amount_moved = speed_factor * dt;
	c_my_transform->getYawPitchRoll(&current_yaw, &current_pitch);

	tower_center.y = myPos.y;
	//float distance = VEC3::Distance(myPos, center);
	VEC3 move_vector = tower_center + myPos;

	if (! c_my_transform->isInFront(ppos)) {
		current_yaw = going_right ? current_yaw - deg2rad(180) : current_yaw + deg2rad(180);
		float debug = rad2deg(current_yaw);
		going_right = !going_right;
		c_my_transform->setYawPitchRoll(current_yaw, current_pitch);
	}
	else {
		current_yaw = going_right ? current_yaw - 0.1 * amount_moved : current_yaw + 0.1 * amount_moved;
		c_my_transform->setYawPitchRoll(current_yaw, current_pitch);
		VEC3 aux_vector = going_right ? -1 * c_my_transform->getLeft() :  c_my_transform->getLeft();
		VEC3 newPos = tower_center + (aux_vector * tower_radius);
		c_my_transform->setYawPitchRoll(current_yaw, current_pitch);
		TCompCollider* comp_collider = get<TCompCollider>();
		if (comp_collider && comp_collider->controller)
		{
			VEC3 delta_move = newPos - myPos;
			distance_to_player = VEC3::Distance(myPos, ppos);
			delta_move.y += -10*dt;
			comp_collider->controller->move(physx::PxVec3(delta_move.x, delta_move.y, delta_move.z), 0.f, dt, physx::PxControllerFilters());
		}
	}


	distance_to_player = VEC3::Distance(myPos, ppos);
	if (distance_to_player < attack_distance + 2.f)
		ChangeState("attack");
	if (distance_to_player > chase_distance + 4.f)
		++aux_count;
		//ChangeState("disappear");
}

void CAIBossRunner::AttackState() {
	change_color(VEC4(0, 0, 0, 1));
	TCompTransform *my_pos = getMyTransform();
	CEntity *player = (CEntity *)getEntityByName("The Player");
	TCompTransform *ppos = player->get<TCompTransform>();
	distance_to_player = VEC3::Distance(my_pos->getPosition(), ppos->getPosition());
	if (distance_to_player > attack_distance + 1.5f)
		ChangeState("chase");
}

void CAIBossRunner::DissapearState() {
	TCompRender *my_render = getMyRender();
	//my_render->is_active = false;
	ChangeState("nextwpt");
}

void CAIBossRunner::DieState() {
	change_color(VEC4(1, 1, 1, 1));
}
