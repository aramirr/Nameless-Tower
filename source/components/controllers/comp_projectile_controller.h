#pragma once

#include "components/comp_base.h"
#include "geometry/transform.h"
#include "entity/common_msgs.h"

class TCompProjectileController : public TCompBase {

  CHandle     h_entity;
	float				time_interval;
	float				current_interval;
	bool				is_active;

  DECL_SIBLING_ACCESS();

public:
  void debugInMenu();
  void load(const json& j, TEntityParseContext& ctx);
  void update(float dt);
	void setEntity(CHandle new_entity);
	void activate(const TMsgActivateProjectile& msg);
	void deactivate(const TMsgDeactivateProjectile& msg);
	static void registerMsgs();
};