#pragma once

#include "components/comp_base.h"


class TCompBoneAnchor : public TCompBase
{
  DECL_SIBLING_ACCESS();

public:
  void debugInMenu();
  void load(const json& j, TEntityParseContext& ctx);
  void update(float DT);

private:
	CHandle skeleton_owner;
	int bone_id;
};

