#pragma once

#include "mcv_platform.h"
#include <lua/SLB/SLB.hpp>
 using namespace SLB;
 
 enum ScriptEvents {
	 trigger_enter = 0,
	 max_num_events
 };

class ScriptingModule
{
	
	public:
		std::string name;
		Manager logic_manager;
		Script script;		

		ScriptingModule(const std::string& name);
		void BootLuaSLB(SLB::Manager *m);
		void ExecEvent(ScriptEvents e, std::vector<std::string> params);

		void fTriggerEnter(std::vector<std::string> params);
};