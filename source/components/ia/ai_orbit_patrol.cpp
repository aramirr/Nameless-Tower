#include "mcv_platform.h"
#include "handle/handle.h"
#include "ai_orbit_patrol.h"
#include "entity/entity_parser.h"
#include "components/comp_transform.h"
#include "render/render_utils.h"

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

	float y, p;
	c_my_transform->getYawPitchRoll(&y, &p);
	float distance = VEC3::Distance(center, myPos);
	c_my_transform->setPosition(center);

	if (move_left == true)
	{
		y -= dt * speed;
	}
	else
	{
		y += dt * speed;
	}
	c_my_transform->setYawPitchRoll(y, p);
	VEC3 newPos = c_my_transform->getPosition() - (c_my_transform->getFront() * distance);
	/*
	VEC3 vertical_distance;
	vertical_distance.y = getWaypoint().y - c_my_transform->getPosition().y;
	if (vertical_distance.y > 0)
	{
		newPos = newPos + (vertical_distance * 0.01 * speed);
	}
	*/
	c_my_transform->setPosition(newPos);
	
	if (VEC3::Distance(getWaypoint(), myPos) < 1)
	{
		acum_delay = 0;
		ChangeState("wait_state");
	}
}

void CAIOrbitPatrol::WaitState(float dt)
{
	acum_delay += dt;
	if (delay < acum_delay) {
		ChangeState("next_waypoint");
	}
}
