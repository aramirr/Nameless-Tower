#include "mcv_platform.h"
#include "comp_render.h"

DECL_OBJ_MANAGER("render", TCompRender);

#include "render/render_objects.h"
#include "render/texture/texture.h"
#include "render/texture/material.h"

void TCompRender::debugInMenu() {
  ImGui::ColorEdit4("Color", &color.x);
}

void TCompRender::loadMesh(const json& j, TEntityParseContext& ctx) {
  // Load some render mesh
  if (j.count("mesh")) {
    std::string name_mesh = j["mesh"];
    mesh = Resources.get(name_mesh)->as<CRenderMesh>();

    std::string name_material = j["material"];
    const CMaterial* material = Resources.get(name_material)->as<CMaterial>();
    materials.push_back(material);
  }
  else {
    mesh = Resources.get("axis.mesh")->as<CRenderMesh>();
    const CMaterial* material = Resources.get("data/materials/solid.material")->as<CMaterial>();
    materials.push_back(material);
  }
}

void TCompRender::load(const json& j, TEntityParseContext& ctx) {

  if (j.is_array()) {
    for (size_t i = 0; i < j.size(); ++i )
      loadMesh(j[i], ctx);
  }

  // If there is a color in the json, read it
  if (j.count("color"))
    color = loadVEC4(j["color"]);

}