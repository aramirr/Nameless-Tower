#include "mcv_platform.h"
#include "logic_manager.h"
#include "components/juan/comp_transform.h"
#include <lua/SLB/SLB.hpp>
using namespace SLB;


LogicManager::LogicManager()
{
	printf("constructor\n");
	numagents = 37;
}



void LogicManager::RespawnPlayer()
{
	printf("Player Respawns \n");
	playerlife = 100;
	px = 0;
	py = 0;
	pz = 0;
}


void LogicManager::TeleportPlayer(float x, float y, float z)
{
	printf("Teleporting player to %f %f %f\n", x, y, z);
	px = x;
	py = y;
	pz = z;
}

void LogicManager::printDbg()
{
	dbg("JOSUE");
}

float LogicManager::GetPlayerLife()
{
	return playerlife;
}

void LogicManager:: shake_body() {
	CEntity* player = getEntityByName("The Player");
	TCompCollider* player_collider = player->get<TCompCollider>();
	//player_collider->controller->move(PxVec3(0, 0.5*dt, 0), 0.f, dt, PxControllerFilters());


}


/*void LogicManager::GetPlayerPos(float &x, float &y, float &z)
{
x = px;
y = py;
z = pz;
}*/

// ARRANQUE DE SLB

