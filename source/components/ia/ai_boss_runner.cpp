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
	AddState("appear", (statehandler)&CAIBossRunner::appear_state);
	AddState("disappear", (statehandler)&CAIBossRunner::disapear_state);
	AddState("chase", (statehandler)&CAIBossRunner::chase_state);
	AddState("attack", (statehandler)&CAIBossRunner::attack_state);
	AddState("jump", (statehandler)&CAIBossRunner::jumping_state);

	// reset the state
	actual_state = "disappear";
	ChangeState("disappear");
}
void CAIBossRunner::appear(const TMsgAppear& msg) {
	if (actual_state == "disappear") {
		appearing_position = msg.appearing_position;
		actual_state = "appear";
		ChangeState("appear");
	}
}

void CAIBossRunner::on_player_jump(const TMsgJump& msg) {
	jump_positions.push(msg.jump_position);
}

void CAIBossRunner::registerMsgs() {
	DECL_MSG(CAIBossRunner, TMsgJump, on_player_jump);
	DECL_MSG(CAIBossRunner, TMsgAppear, appear);
}

void CAIBossRunner::debugInMenu() {
	IAIController::debugInMenu();
	ImGui::Text("Distance player %f", distance_to_player);
	ImGui::DragFloat("Jump speed: %f", &jump_speed, 0.01f, 0.f, 100.f);
}

void CAIBossRunner::load(const json& j, TEntityParseContext& ctx) {
	setEntity(ctx.current_entity);
	Init();
	chase_distance = j.value("chase_distance", 3.0f);
	attack_distance = j.value("attack_distance", 1.0f);
	speed_factor = j.value("speed_factor", 5.0f);
	tower_radius = j.value("tower_radius", 15.0f);
	gravity = j.value("gravity", 16.5f);
	jump_speed = j.value("jump_speed", 25.8f);
}

void CAIBossRunner::appear_state(float dt) {
	TCompTransform* my_transform = getMyTransform();
	VEC3 delta_move = appearing_position - my_transform->getPosition();
	TCompCollider* my_collider = get<TCompCollider>();
	my_collider->controller->move(physx::PxVec3(delta_move.x, delta_move.y, delta_move.z), 0.f, dt, physx::PxControllerFilters());
	my_transform->lookAt(appearing_position, tower_center);
	float y, p, r;
	my_transform->getYawPitchRoll(&y, &p, &r);
	y -= deg2rad(90);
	my_transform->setYawPitchRoll(y, p, r);
	change_color(VEC4(0, 1, 1, 1));
	going_right = my_transform->isInLeft(tower_center);
	TCompRender *my_render = getMyRender();
	my_render->is_active = true;
	actual_state = "chase";
 	ChangeState("chase");
}

void CAIBossRunner::chase_state(float dt) {
	change_color(VEC4(1, 1, 0, 1));
	TCompTransform *c_my_transform = getMyTransform();
	VEC3 myPos = c_my_transform->getPosition();
	CEntity *player = (CEntity *)getEntityByName("The Player");
	TCompTransform *c_p_transform = player->get<TCompTransform>();
	VEC3 ppos = c_p_transform->getPosition();

	if (!jump_positions.empty() && VEC3::Distance(myPos, jump_positions.front()) < 1.f) {
		jump_end = c_my_transform->getPosition().y + 5.f;
		jump_positions.pop();
		ChangeState("jump");
	}

	float current_yaw;
	float current_pitch;
	float amount_moved = speed_factor * dt;
	c_my_transform->getYawPitchRoll(&current_yaw, &current_pitch);

	tower_center.y = myPos.y;

	if (!c_my_transform->isInFront(ppos)) {
		current_yaw = going_right ? current_yaw - deg2rad(180) : current_yaw + deg2rad(180);
		float debug = rad2deg(current_yaw);
		going_right = !going_right;
		c_my_transform->setYawPitchRoll(current_yaw, current_pitch);
	}
	else {
		current_yaw = going_right ? current_yaw + 0.1 * amount_moved : current_yaw - 0.1 * amount_moved;
		c_my_transform->setYawPitchRoll(current_yaw, current_pitch);
		VEC3 aux_vector = going_right ? -1 * c_my_transform->getLeft() : c_my_transform->getLeft();
		VEC3 newPos = tower_center + (aux_vector * tower_radius);
		c_my_transform->setYawPitchRoll(current_yaw, current_pitch);
		TCompCollider* comp_collider = get<TCompCollider>();
		if (comp_collider && comp_collider->controller)
		{
			VEC3 delta_move = newPos - myPos;
			distance_to_player = VEC3::Distance(myPos, ppos);
			delta_move.y += -10 * dt;
			comp_collider->controller->move(physx::PxVec3(delta_move.x, delta_move.y, delta_move.z), 0.f, dt, physx::PxControllerFilters());
		}
	}

	distance_to_player = VEC3::Distance(myPos, ppos);
	if (distance_to_player < attack_distance + 2.f) {
		actual_state = "attack";
		ChangeState("attack");
	}
	if (distance_to_player > chase_distance + 10.f) {
		actual_state = "disappear";
		ChangeState("disappear");
	}
}

void CAIBossRunner::attack_state() {
	change_color(VEC4(0, 0, 0, 1));
	TCompTransform *my_pos = getMyTransform();
	CEntity *player = (CEntity *)getEntityByName("The Player");
	TCompTransform *ppos = player->get<TCompTransform>();
	distance_to_player = VEC3::Distance(my_pos->getPosition(), ppos->getPosition());
	if (distance_to_player > attack_distance + 2.5f) {
		actual_state = "chase";
		ChangeState("chase");
	}
}

void CAIBossRunner::disapear_state() {
	TCompRender *my_render = getMyRender();
	//my_render->is_active = false;
	change_color(VEC4(0, 1, 0, 1));
	jump_positions = std::queue<VEC3>();
}

void CAIBossRunner::jumping_state(float dt) {
	change_color(VEC4(1, 0, 1, 1));
	TCompTransform *c_my_transform = get<TCompTransform>();
	VEC3 my_pos = c_my_transform->getPosition();
	VEC3 new_pos = my_pos;
	new_pos.y += (jump_speed - gravity * dt) * dt;
	tower_center.y = my_pos.y;
	// Chequea que no llego a la altura maxima 
	if (new_pos.y < jump_end) {
		float current_yaw, current_pitch;
		c_my_transform->getYawPitchRoll(&current_yaw, &current_pitch);
		float amount_moved = speed_factor * dt;
		current_yaw = going_right ? current_yaw + 0.1 * amount_moved : current_yaw - 0.1 * amount_moved;
		c_my_transform->setYawPitchRoll(current_yaw, current_pitch);
		VEC3 aux_vector = going_right ? -1 * c_my_transform->getLeft() : c_my_transform->getLeft();
		new_pos = tower_center + (aux_vector * tower_radius);
		c_my_transform->setYawPitchRoll(current_yaw, current_pitch);
		TCompCollider* comp_collider = get<TCompCollider>();
		if (comp_collider && comp_collider->controller)
		{
			VEC3 delta_move = new_pos - my_pos;
			delta_move.y += -(-(jump_speed - gravity * dt)) * dt;
			comp_collider->controller->move(physx::PxVec3(delta_move.x, delta_move.y, delta_move.z), 0.f, dt, physx::PxControllerFilters());
		}
	}
	else {
		ChangeState("chase");
	}
}