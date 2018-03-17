#pragma once

#include "components\comp_base.h"
#include "entity/common_msgs.h"
#include "components/juan/comp_transform.h"


class TCompArrowUI : public TCompBase {
public:
	VEC2 start_pos;
	VEC3 unit_force_vector;
	float vector_module;
	boolean available_capture = 1;
	CHandle         h_entity;

	void debugInMenu();
	void update(float dt);
	void load(const json& j, TEntityParseContext& ctx);

};