#include "mcv_platform.h"
#include "handle/handle.h"
#include "ai_linear_patrol.h"
#include "entity/entity_parser.h"
#include "components/comp_transform.h"
#include "render/render_utils.h"

DECL_OBJ_MANAGER("ai_linear_patrol", CAILinearPatrol);

void CAILinearPatrol::Init()
{
	// insert all states in the map
	AddState("initialize_waypoint", (statehandler)&CAILinearPatrol::InitializeWaypointState);
	AddState("next_waypoint", (statehandler)&CAILinearPatrol::NextWaypointState);
	AddState("move_to_waypoint", (statehandler)&CAILinearPatrol::MoveToWaypointState);
	AddState("wait_state", (statehandler)&CAILinearPatrol::WaitState);

	// reset the state
	ChangeState("initialize_waypoint");
	currentWaypoint = 0;
}

void CAILinearPatrol::debugInMenu() {

	IAIController::debugInMenu();
	TCompTransform *mypos = getMyTransform();
	VEC3 vp = mypos->getPosition();
	ImGui::Text("Curr Waypoint %d", currentWaypoint);
	ImGui::DragFloat("Speed %f", &speed);
	ImGui::DragFloat("Delay %f", &delay);
	ImGui::Text("Acum Delay %f", acum_delay);
	ImGui::Text("Distance %f", VEC3::Distance(getWaypoint(), vp));
}

void CAILinearPatrol::load(const json& j, TEntityParseContext& ctx) {
	setEntity(ctx.current_entity);

	Init();

	speed = j.value("speed", 2.0f);
	delay = j.value("delay", 2.0f);

	auto& j_waypoints = j["waypoints"];
	for (auto it = j_waypoints.begin(); it != j_waypoints.end(); ++it) {
		VEC3 p = loadVEC3(it.value());
		addWaypoint(p);
	}
}



void CAILinearPatrol::InitializeWaypointState()
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
	ChangeState("move_to_waypoint");
}

void CAILinearPatrol::NextWaypointState()
{
	currentWaypoint = (currentWaypoint + 1) % _waypoints.size();
	ChangeState("move_to_waypoint");
}

void CAILinearPatrol::MoveToWaypointState()
{
	TCompTransform *mypos = getMyTransform();
	VEC3 wppos = getWaypoint();

	VEC3 dir = wppos - mypos->getPosition();
	dir.Normalize();
	VEC3 delta_pos = mypos->getPosition() + speed * dir;
	mypos->setPosition(delta_pos);
	
	if (VEC3::Distance(getWaypoint(), mypos->getPosition()) < 1)
	{
		acum_delay = 0;
		ChangeState("wait_state");
	}
	
}

void CAILinearPatrol::WaitState(float dt)
{
	acum_delay += dt;
	if (delay < acum_delay) {
		ChangeState("next_waypoint");
	}
}
