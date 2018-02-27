#pragma once

#include "comp_base.h"
#include "geometry/transform.h"
#include "ia/ai_controller.h"

class TCompTrigger : public IAIController {
	enum TriggerTypes {player, runner, plattform_orbit, spikes, none};
	std::string trigger_type;
	std::string collider_entity;

	CHandle h_other_entity;
	VEC3 appearing_position;
	CHandle         h_entity;

	void onTriggerEnter(const TMsgTriggerEnter& msg);
	void onTriggerExit(const TMsgTriggerExit& msg);
	
	DECL_SIBLING_ACCESS();

public:
	static void registerMsgs();
	void debugInMenu();
	void load(const json& j, TEntityParseContext& ctx);
	void update(float dt);
};