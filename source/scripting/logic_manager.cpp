#include "mcv_platform.h"
#include <lua/SLB/SLB.hpp>

#include "logic_manager.h"
#include "components/juan/comp_transform.h"
#include "components/juan/comp_render.h"
using namespace SLB;


LogicManager::LogicManager()
{
	
}


void LogicManager::printDbg()
{
	dbg("JOSELITO");
}


void LogicManager::disappearEntity(const char* name) {
	EngineTower.disappearEntity(name);
}

void LogicManager::appearEntity(const char* name) {
	EngineTower.appearEntity(name);
}

void LogicManager::activateCamera(const char* name) {
	dbg("JOHAN");
}

