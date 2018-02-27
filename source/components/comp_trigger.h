#pragma once

#include "comp_base.h"
#include "geometry/transform.h"
#include "ia/ai_controller.h"

class TCompTrigger : public IAIController {
	enum TriggerTypes {player, runner, none};
	std::string trigger_type;

	CHandle h_other_entity;
	VEC3 appearing_position;

	void onTriggerEnter(const TMsgTriggerEnter& msg);

	DECL_SIBLING_ACCESS();

public:
	static void registerMsgs();
	void debugInMenu();
	void load(const json& j, TEntityParseContext& ctx);
	void update(float dt);
};