#pragma once

#include "components/comp_base.h"
#include "geometry/transform.h"
#include "entity/common_msgs.h"

class TCompSpiralController : public TCompBase {

  float       radius;
  CHandle     h_entity;
	CHandle			h_parent;
  float       speed;
  VEC3		  center;
  VEC3		  direction;
  float		  offsetY;
	bool			set_once = false;
	bool			is_left;

  DECL_SIBLING_ACCESS();

public:
	static void registerMsgs();
  void debugInMenu();
  void load(const json& j, TEntityParseContext& ctx);
  void update(float dt);
  void setEntity(CHandle new_entity);
	void setDirection(const TMsgChangeDirection& msg);
};