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
	TCompRender *my_render = getMyRender();
	my_render->color = VEC4(1, 1, 0, 1);
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
	TCompRender *my_render = getMyRender();
	my_render->is_active = true;
	ChangeState("chase");
}

void CAIBossRunner::ChaseState(float dt) {
	TCompTransform *my_pos = getMyTransform();
	CEntity *player = (CEntity *)getEntityByName("The Player");
	TCompTransform *ppos = player->get<TCompTransform>();

	TCompRender *my_render = getMyRender();
	my_render->color = VEC4(0, 1, 0, 1);

	float amount_moved = speed_factor * dt;
	
	float y, p, r;
	my_pos->getYawPitchRoll(&y, &p, &r);
	float distance_center = VEC3::Distance(my_pos->getPosition(), tower_center);
	if (going_right) {
		if (my_pos->isInFront(ppos->getPosition())) {
			my_pos->setPosition(tower_center);
			y += 0.1 * amount_moved;
			my_pos->setYawPitchRoll(y, p, r);
			my_pos->setPosition(my_pos->getPosition() - my_pos->getLeft() * distance_center);
		}
		else {
			going_right = false;
			y += deg2rad(180);
			my_pos->setYawPitchRoll(y, p, r);
		}
	}
	else {
		if (my_pos->isInFront(ppos->getPosition())) {
			my_pos->setPosition(tower_center);
			y -= 0.1 * amount_moved;
			my_pos->setYawPitchRoll(y, p, r);
			my_pos->setPosition(my_pos->getPosition() + my_pos->getLeft() * distance_center);
		}
		else {
			going_right = true;
			y += deg2rad(180);
			my_pos->setYawPitchRoll(y, p, r);
		}
	}
	VEC3 a = my_pos->getPosition();
	VEC3 b = ppos->getPosition();
	distance_to_player = VEC3::Distance(a, b);
	if (distance_to_player < attack_distance)
		ChangeState("attack");
	if (distance_to_player > chase_distance + 0.5f)
		ChangeState("disappear");
}

void CAIBossRunner::AttackState() {
	TCompRender *my_render = getMyRender();
	my_render->color = VEC4(0, 0, 0, 1);
	TCompTransform *my_pos = getMyTransform();
	CEntity *player = (CEntity *)getEntityByName("The Player");
	TCompTransform *ppos = player->get<TCompTransform>();
	distance_to_player = VEC3::Distance(my_pos->getPosition(), ppos->getPosition());
	if (distance_to_player > attack_distance + 0.5f)
		ChangeState("chase");
}

void CAIBossRunner::DissapearState() {
	TCompRender *my_render = getMyRender();
	//my_render->is_active = false;
	ChangeState("nextwpt");
}

void CAIBossRunner::DieState() {
	TCompRender *my_render = getMyRender();
	my_render->color = VEC4(1, 1, 1, 1);
}
