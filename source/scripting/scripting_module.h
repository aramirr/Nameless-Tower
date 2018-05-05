#pragma once

#include "mcv_platform.h"
#include <lua/SLB/SLB.hpp>
 using namespace SLB;

class ScriptingModule
{
	
	public:
		ScriptingModule(const std::string& name);

		Manager logic_manager;
		Script *script;
		std::string name;
		int auxiliar_bdg = 0;
		void boot();
		void BootLuaSLB(SLB::Manager *m);
};