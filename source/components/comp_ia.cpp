#include "mcv_platform.h"
#include "comp_ia.h"
#include "modules/module_ia.h"

DECL_OBJ_MANAGER("ia", TCompIA);

void TCompIA::debugInMenu() {
}

void TCompIA::load(const json& j, TEntityParseContext& ctx) {
  ai_controller = CEngine::get().getIA().getNewAIControler(j, nullptr );
}