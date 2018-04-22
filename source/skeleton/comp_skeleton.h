#ifndef INC_COMP_SKELETON_H_
#define INC_COMP_SKELETON_H_

#include "../components/comp_base.h"
#include "render/cte_buffer.h"
#include "ctes.h"

class CalModel;

struct TCompSkeleton : public TCompBase {
  CRenderCte<CCteSkinBones> cb_bones;
  CalModel* model = nullptr;
  TCompSkeleton();
  ~TCompSkeleton();
  void load(const json& j, TEntityParseContext& ctx);
  void update(float dt);
  void debugInMenu();
  void renderDebug();
  void updateCtesBones();
  void playAnimation(int id_amimation, bool is_action, float in_delay, float out_delay);
  void deleteAnimation(int id_animation, bool is_action, float out_delay);
  DECL_SIBLING_ACCESS();
};

#endif
