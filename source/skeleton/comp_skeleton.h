#ifndef INC_COMP_SKELETON_H_
#define INC_COMP_SKELETON_H_

#include "../components/comp_base.h"

class CalModel;

struct TCompSkeleton : public TCompBase {
  CalModel* model = nullptr;
  void load(const json& j, TEntityParseContext& ctx);
  void update(float dt);
  void debugInMenu();
  void renderDebug();
};

#endif
