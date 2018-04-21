#include "mcv_platform.h"
#include "handle/handle.h"
#include "ai_orbit_patrol.h"
#include "entity/entity_parser.h"
#include "components/juan/comp_transform.h"
#include "render/render_utils.h"
#include "modules/system/module_physics.h"
#include "components/player/comp_player_controller.h"
#include "components/physics/controller_filter.h"

using namespace physx;

DECL_OBJ_MANAGER("ai_orbit_patrol", CAIOrbitPatrol);

void CAIOrbitPatrol::Init()
{
	// insert all states in the map
	AddState("initialize_waypoint", (statehandler)&CAIOrbitPatrol::InitializeWaypointState);
	AddState("next_waypoint", (statehandler)&CAIOrbitPatrol::NextWaypointState);
	AddState("move_to_waypoint", (statehandler)&CAIOrbitPatrol::MoveToWaypointState);
	AddState("wait_state", (statehandler)&CAIOrbitPatrol::WaitState);

	// reset the state
	ChangeState("initialize_waypoint");
}

void CAIOrbitPatrol::debugInMenu() {

	IAIController::debugInMenu();
	TCompTransform *mypos = getMyTransform();
	VEC3 vp = mypos->getPosition();
	ImGui::Text("Curr Waypoint %d", currentWaypoint);
	ImGui::DragFloat("Speed %f", &speed);
	ImGui::DragFloat("Delay %f", &delay);
	ImGui::Text("Acum Delay %f", acum_delay);
	ImGui::Text("Distance %f", VEC3::Distance(getWaypoint(), vp));
	ImGui::DragFloat3("Center", &center.x, 0.1f, -20.f, 20.f);
	ImGui::DragFloat("Radius %f", &radius);
}

VEC3 CAIOrbitPatrol::processWaypoint(const VEC3 center, const VEC3 waypoint, const float distance) {
	//VEC3 vector = waypoint - center;
	VEC2 vector2D = VEC2{ waypoint.x, waypoint.z } -VEC2{ center.x, center.z };
	vector2D.Normalize();
	VEC3 result = VEC3{ (vector2D.x * distance), (waypoint.y), (vector2D.y * distance) };
	return result;
}

void CAIOrbitPatrol::load(const json& j, TEntityParseContext& ctx) {
	setEntity(ctx.current_entity);

	speed = j.value("speed", 2.0f);
	delay = j.value("delay", 2.0f);
	center = loadVEC3(j["center"]);
	radius = j["radius"];

	auto& j_waypoints = j["waypoints"];
	for (auto it = j_waypoints.begin(); it != j_waypoints.end(); ++it) {
		VEC3 p = loadVEC3(it.value());
		VEC3 processed_waypoint = processWaypoint(center, p, radius);
		addWaypoint(processed_waypoint);
	}

	Init();
}


void CAIOrbitPatrol::InitializeWaypointState()
{
	TCompTransform *mypos = getMyTransform();
	float current_distance;
	int current_index;
	for (int i = 0; i < _waypoints.size(); i++)
	{
		if (i == 0)
		{
			current_distance = VEC3::Distance(mypos->getPosition(), _waypoints[i]);
			current_index = 0;
		}
		else
		{
			float calculated_distance = VEC3::Distance(mypos->getPosition(), _waypoints[i]);
			if (calculated_distance < current_distance)
			{
				current_distance = calculated_distance;
				current_index = i;
			}
		}
	}
	currentWaypoint = current_index;
	TCompTransform *c_my_transform = get<TCompTransform>();
	move_left = c_my_transform->isInLeft(getWaypoint());
	ChangeState("move_to_waypoint");
}

void CAIOrbitPatrol::NextWaypointState()
{
	currentWaypoint = (currentWaypoint + 1) % _waypoints.size();
	TCompTransform *c_my_transform = get<TCompTransform>();
	move_left = c_my_transform->isInLeft(getWaypoint());
	ChangeState("move_to_waypoint");
}

void CAIOrbitPatrol::MoveToWaypointState(float dt)
{
	
	TCompTransform *c_my_transform = get<TCompTransform>();
	VEC3 myPos = c_my_transform->getPosition();
	QUAT myRot = c_my_transform->getRotation();

	float y, p;
	c_my_transform->getYawPitchRoll(&y, &p);
	float distance = VEC3::Distance(center, myPos);
	c_my_transform->setPosition(center);

	if (move_left == true)
	{
		y -= DT * speed;
	}
	else
	{
		y += DT * speed;
	}
	c_my_transform->setYawPitchRoll(y, p);
	VEC3 newPos = c_my_transform->getPosition() - (c_my_transform->getFront() * distance);
	c_my_transform->setPosition(newPos);
	QUAT newRot = c_my_transform->getRotation();

	TCompCollider* comp_collider = get<TCompCollider>();
	if (comp_collider)
	{
		PxRigidActor* rigidActor = ((PxRigidActor*)comp_collider->actor);
		PxTransform tr = rigidActor->getGlobalPose();
		tr.p = PxVec3(newPos.x, newPos.y, newPos.z); 
		tr.q = PxQuat(newRot.x, newRot.y, newRot.z, newRot.w);
		rigidActor->setGlobalPose(tr);
		if (attached.isValid()) {
			CEntity* e = attached;
			assert(e);
			TCompCollider *player_collider = e->get< TCompCollider >();
			TCompTransform *player_transform = e->get< TCompTransform >();
			VEC3 delta_pos = newPos - myPos;
			float p_y, p_p;
			player_transform->getYawPitchRoll(&p_y, &p_p);

			if (move_left == true)
			{
				p_y -= DT * speed;
			}
			else
			{
				p_y += DT * speed;
			}
			player_transform->setYawPitchRoll(p_y, p_p);


			PxShape* player_shape;
			comp_collider->controller->getActor()->getShapes(&player_shape, 1);
			PxFilterData filter_data = player_shape->getSimulationFilterData();
			ControllerFilterCallback *filter_controller = new ControllerFilterCallback();
			player_collider->controller->move(PxVec3(delta_pos.x, delta_pos.y, delta_pos.z), 0.f, DT, PxControllerFilters(&filter_data, filter_controller, filter_controller));
			
			TCompPlayerController *player_controller = e->get<TCompPlayerController>();
			VEC3 tower_center = player_controller->center;
			VEC3 player_pos = player_transform->getPosition();
			float d = VEC3::Distance({ tower_center.x, 0, tower_center.z }, { player_pos.x, 0, player_pos.z });
			if (d != player_controller->tower_radius)
			{
				VEC3 d_vector = player_pos - tower_center;
				d_vector.Normalize();
				VEC3 new_pos = d_vector * player_controller->tower_radius;

			}
		}
	}
	
	if (VEC3::Distance(getWaypoint(), myPos) < 2.5)
	{
		acum_delay = 0;
		ChangeState("wait_state");
	}
}

void CAIOrbitPatrol::WaitState(float dt)
{
	acum_delay += DT;
	if (delay < acum_delay) {
		ChangeState("next_waypoint");
	}
}


void CAIOrbitPatrol::registerMsgs() {
DECL_MSG(CAIOrbitPatrol, TMsgAttachTo, attachPlayer);
DECL_MSG(CAIOrbitPatrol, TMsgDetachOf, detachPlayer);
}

void CAIOrbitPatrol::attachPlayer(const TMsgAttachTo& msg) {
	attached = msg.h_attacher;
}

void CAIOrbitPatrol::detachPlayer(const TMsgDetachOf& msg) {
	attached = CHandle();
}
