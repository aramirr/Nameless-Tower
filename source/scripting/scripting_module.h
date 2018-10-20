#pragma once

#include "mcv_platform.h"
#include <lua/SLB/SLB.hpp>
 using namespace SLB;
 
 enum ScriptEvents {
	 trigger_enter = 0,
     windstrike_activate = 1,
     trigger_exit = 2,
	 max_num_events
 };

class ScriptingModule
{
	bool done_cinematic_monolito = false;

	public:
		std::string name;
		Manager logic_manager;
		Script script;		

		ScriptingModule(const std::string& name);
		void BootLuaSLB(SLB::Manager *m);
		void ExecEvent(ScriptEvents e, std::vector<std::string> params);

    void fTriggerExit(std::vector<std::string> params);
    void fWindstrikeActivate(std::vector<std::string> params);
};