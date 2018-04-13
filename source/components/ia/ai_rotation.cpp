#include "mcv_platform.h"
#include "handle/handle.h"
#include "ai_rotation.h"
#include "entity/entity_parser.h"
#include "components/juan/comp_transform.h"
#include "render/render_utils.h"
#include "modules/system/module_physics.h"
#include "components/player/comp_player_controller.h"

using namespace physx;

DECL_OBJ_MANAGER("ai_rotator", CAIRotator);

void CAIRotator::Init()
{
	// insert all states in the map
	AddState("next_config_state", (statehandler)&CAIRotator::NextConfigState);
	AddState("rotate_state", (statehandler)&CAIRotator::RotateState);
	AddState("stop_state", (statehandler)&CAIRotator::StopState);

	// reset the state
	it_config = 0;
	current_radiants = 0.f;
	current_time = 0.f;
	ChangeState("rotate_state");
}

void CAIRotator::debugInMenu() {

	IAIController::debugInMenu();
	ImGui::DragFloat("IT %f", &it_config);
	ImGui::DragFloat("Rad %f", &current_radiants);
	float deg = rad2deg(current_radiants);
	ImGui::DragFloat("Deg %f", &deg);
	ImGui::DragFloat("Time %f", &current_time);
}

void CAIRotator::load(const json& j, TEntityParseContext& ctx) {
	setEntity(ctx.current_entity);

	auto& j_configs = j["configs"];
	for (auto it = j_configs.begin(); it != j_configs.end(); ++it) {
		TConfig conf;
		if (it.value()["axis"] == "X") conf.axis = X;
		else if (it.value()["axis"] == "Y") conf.axis = Y;
		else conf.axis = Z;

		conf.increase = it.value()["increase"];
		conf.speed = it.value()["speed"];
		conf.wait_time = it.value()["wait_time"];
		conf.radiants = it.value()["degrees"];
		conf.radiants = deg2rad(conf.radiants);
		config_states.push_back(conf);
	}

	Init();
}

void CAIRotator::NextConfigState() {
	++it_config;
	if (it_config >= config_states.size()) it_config = 0;
	current_time = 0.f;
	current_radiants = 0.f;
	ChangeState("rotate_state");
};

void CAIRotator::RotateState(float dt) {
	TCompCollider *my_collider = getMyCollider();
	TCompTransform *my_transform = getMyTransform();
	VEC3 my_pos = my_transform->getPosition();

	float y, p, r;
	my_transform->getYawPitchRoll(&y, &p, &r);

	float rotation = config_states[it_config].speed * dt;
	if (config_states[it_config].axis == X) {
		change_color(VEC4(1, 0, 0, 0));
		if (config_states[it_config].increase) {
			if (current_radiants + rotation > config_states[it_config].radiants) {
				r += config_states[it_config].radiants - current_radiants;
				current_radiants = config_states[it_config].radiants;
			}
			else {
				r += rotation;
				current_radiants += rotation;
			}
		}
		else {
			if (current_radiants + rotation > config_states[it_config].radiants) {
				r -= config_states[it_config].radiants - current_radiants;
				current_radiants = config_states[it_config].radiants;
			}
			else {
				r -= rotation;
				current_radiants += rotation;
			}
		}
	}
	else if (config_states[it_config].axis == Y) {
		change_color(VEC4(0, 1, 0, 0));

		if (config_states[it_config].increase) {
			if (current_radiants + rotation > config_states[it_config].radiants) {
				y += config_states[it_config].radiants - current_radiants;
				current_radiants = config_states[it_config].radiants;
			}
			else {
				y += rotation;
				current_radiants += rotation;
			}
		}
		else {
			if (current_radiants + rotation > config_states[it_config].radiants) {
				y -= config_states[it_config].radiants - current_radiants;
				current_radiants = config_states[it_config].radiants;
			}
			else {
				y -= rotation;
				current_radiants += rotation;
			}
		}
	}
	else {
		change_color(VEC4(0, 0, 1, 0));
		if (config_states[it_config].increase) {
			if (current_radiants + rotation > config_states[it_config].radiants) {
				p += config_states[it_config].radiants - current_radiants;
				current_radiants = config_states[it_config].radiants;
			}
			else {
				p += rotation;
				current_radiants += rotation;
			}
		}
		else {
			if (current_radiants + rotation > config_states[it_config].radiants) {
				p -= config_states[it_config].radiants - current_radiants;
				current_radiants = config_states[it_config].radiants;
			}
			else {
				p -= rotation;
				current_radiants += rotation;
			}
		}
	}


	my_transform->setYawPitchRoll(y, p, r);
	QUAT newRot = my_transform->getRotation();

	PxRigidActor* rigidActor = ((PxRigidActor*)my_collider->actor);
	PxTransform tr = rigidActor->getGlobalPose();
	tr.p = PxVec3(my_pos.x, my_pos.y, my_pos.z);
	tr.q = PxQuat(newRot.x, newRot.y, newRot.z, newRot.w);
	rigidActor->setGlobalPose(tr);

	if (current_radiants >= config_states[it_config].radiants) {
		current_time = 0.f;
		ChangeState("stop_state");
	}

};

void CAIRotator::StopState(float dt) {
	current_time += dt;
	if (current_time >= config_states[it_config].wait_time)
		ChangeState("next_config_state");
};


void CAIRotator::registerMsgs() {
DECL_MSG(CAIRotator, TMsgAttachTo, attachPlayer);
DECL_MSG(CAIRotator, TMsgDetachOf, detachPlayer);
}

void CAIRotator::attachPlayer(const TMsgAttachTo& msg) {
	attached = msg.h_attacher;
}

void CAIRotator::detachPlayer(const TMsgDetachOf& msg) {
	attached = CHandle();
}
