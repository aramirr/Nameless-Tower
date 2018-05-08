#ifndef INC_COMP_SKEL_LOOKAT_H_
#define INC_COMP_SKEL_LOOKAT_H_

#include "../components/comp_base.h"
#include "../entity/entity.h"

class CalModel;
struct TCompSkeleton;

struct TCompSkelLookAt : public TCompBase {
  CHandle     h_skeleton;        // Handle to comp_skeleton of the entity being tracked

  struct TCorrection {
    std::string bone_name;
    int         bone_id = -1;
    VEC3        axis = VEC3(0,1,0);
    float       angle = 0.f;

    VEC3        local_axis_to_correct;  // 0,1,-0.2
    float       rotation_amount;        // In case we want to rotate not the 100%

    void load(const json& j);
    void debugInMenu();
    void apply(TCompSkeleton* skel, VEC3 world_target);
    void renderDebug();
  };

  TCorrection correction;
  VEC3        target;

  void debugInMenu();
  void load(const json& j, TEntityParseContext& ctx);
  void update(float dt);
  void renderDebug();
  DECL_SIBLING_ACCESS();
};

#endif
