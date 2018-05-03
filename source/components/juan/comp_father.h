
#include "geometry/transform.h"


struct TCompFather : public TCompBase {
  VHandles sons;
  
  void load(const json& j, TEntityParseContext& ctx);
  void update(float dt);
  void debugInMenu();
  static void registerMsgs();
  DECL_SIBLING_ACCESS();
};
