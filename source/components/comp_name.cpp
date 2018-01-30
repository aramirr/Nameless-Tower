#include "mcv_platform.h"
#include "comp_name.h"
#include "handle/handle.h"

DECL_OBJ_MANAGER("name", TCompName);

void TCompName::debugInMenu() {
  ImGui::Text(name);
}

void TCompName::load(const json& j, TEntityParseContext& ctx) {
  auto str = j.get<std::string>();
  setName(str.c_str());
}