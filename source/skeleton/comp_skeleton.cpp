#include "mcv_platform.h"
#include "comp_skeleton.h"
#include "game_core_skeleton.h"
#include "cal3d/cal3d.h"
#include "resources/resources_manager.h"
#include "render/render_utils.h"

DECL_OBJ_MANAGER("skeleton", TCompSkeleton);

void TCompSkeleton::load(const json& j, TEntityParseContext& ctx) {

  CalLoader::setLoadingMode(LOADER_ROTATE_X_AXIS | LOADER_INVERT_V_COORD);

  std::string skel_name= j.value("skeleton", "");
  assert(!skel_name.empty());
  auto res_skel = Resources.get(skel_name)->as< CGameCoreSkeleton >();
  CalCoreModel* core_model = const_cast<CGameCoreSkeleton*>(res_skel);
  model = new CalModel(core_model);
  
  // Play the first animation, at weight 100%, now!
  model->getMixer()->blendCycle(0, 1.0f, 0.f);
  
  // Do a time zero update just to have the bones in a correct place
  model->update(0.f);
}

void TCompSkeleton::update(float dt) {
  assert(model);
  model->update(dt);
}

void TCompSkeleton::debugInMenu() {

}

void TCompSkeleton::renderDebug() {
  VEC3 lines[1024][2];
  int nrLines = model->getSkeleton()->getBoneLines(&lines[0][0].x);
  for (int currLine = 0; currLine < nrLines; currLine++)
    renderLine(lines[currLine][0], lines[currLine][1], VEC4(1, 1, 1, 1));
}
