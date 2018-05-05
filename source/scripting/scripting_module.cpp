#include "mcv_platform.h"
#include "scripting_module.h"
#include "logic_manager.h"
#include <lua/SLB/SLB.hpp>
using namespace SLB;

ScriptingModule::ScriptingModule(const std::string& name)
{
	this->name = name;
	BootLuaSLB(&logic_manager);
	Script s(&logic_manager);
	script = &s;
	script->doFile("data/scripts/test.lua");
}

void ScriptingModule::boot() {
	std::string aux = name;
	auto p1 = script->exists("Test");
	auto p2 = script->exists("OnPlayerKilled");
	auto p3 = script->exists("OnPlayerKilled");
	auxiliar_bdg = 1;
}

void ScriptingModule::BootLuaSLB(SLB::Manager *m)
{
	SLB::Class< LogicManager >("LogicManager", m)
		// a comment/documentation for the class [optional]
		.comment("This is our wrapper of LogicManager class")
		// empty constructor, we can also wrapper constructors
		// with arguments using .constructor<TypeArg1,TypeArg2,..>()
		.constructor()
		// a method/function/value...
		.set("RespawnPlayer", &LogicManager::RespawnPlayer)
		.set("TeleportPlayer", &LogicManager::TeleportPlayer)
		.set("GetPlayerLife", &LogicManager::GetPlayerLife)
		.set("printdbg", &LogicManager::printDbg)
		//.set("GetPlayerPos", &LogicManager::GetPlayerPos)
		.property("numagents", &LogicManager::numagents)
		;
}