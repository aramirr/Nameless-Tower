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
	y += deg2rad(90);
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
	CEntity *player = (CEntity *)getEntityByName("The Player");
	TCompTransform *c_p_transform = player->get<TCompTransform>();
	VEC3 ppos = c_p_transform->getPosition();

	change_color(VEC4(0, 1, 0, 1));

	float amount_moved = speed_factor * dt;
	tower_center.y = c_my_transform->getPosition().y;
	float y, p, r;
	c_my_transform->getYawPitchRoll(&y, &p, &r);

	if (c_my_transform->isInFront(ppos)) {
		y = going_right ? y + 0.1*amount_moved : y - 0.1*amount_moved;
		c_my_transform->setYawPitchRoll(y, p, r);
		VEC3 new_position = going_right ? tower_center - c_my_transform->getLeft() * tower_radius : tower_center + c_my_transform->getLeft() * tower_radius;

		VEC3 delta_move = new_position - c_my_transform->getPosition();
		delta_move.y += -10 * dt;
		TCompCollider* comp_collider = get<TCompCollider>();
		comp_collider->controller->move(physx::PxVec3(delta_move.x, delta_move.y, delta_move.z), 0.f, dt, physx::PxControllerFilters());

	}
	else {
		y += deg2rad(180);
		c_my_transform->setYawPitchRoll(y, p, r);
		going_right = !going_right;
	}
	
	VEC3 a = c_my_transform->getPosition();
	VEC3 b = ppos;
	distance_to_player = VEC3::Distance(a, b);
	if (distance_to_player < attack_distance)
		ChangeState("attack");
	if (distance_to_player > chase_distance +4.f)
		ChangeState("disappear");
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
