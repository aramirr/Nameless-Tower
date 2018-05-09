#include "mcv_platform.h"
#include "comp_render_outlines.h"
#include "render/texture/render_to_texture.h"
#include "resources/resources_manager.h"
#include "render/render_objects.h"

DECL_OBJ_MANAGER("render_outlines", TCompRenderOutlines);

// ---------------------
void TCompRenderOutlines::debugInMenu() {
  ImGui::Checkbox("Enabled", &enabled);
  ImGui::DragFloat("Amount", &amount, 0.01f, 0.0f, 1.0f);
}

void TCompRenderOutlines::load(const json& j, TEntityParseContext& ctx) {
  enabled = j.value("enabled", true);
  amount= j.value( "amount", 1.0f);
  tech = Resources.get("outlines.tech")->as<CRenderTechnique>();
  mesh = Resources.get("unit_quad_xy.mesh")->as<CRenderMesh>();
}

void TCompRenderOutlines::apply( ) {
  if (!enabled)
    return;
  CTraceScoped scope("TCompRenderOutlines");

  // Upload to the GPU the how much visibe is the effect (defaults to 100%)
  cb_globals.global_shared_fx_amount = amount;
  cb_globals.updateGPU();

  //CRenderToTexture* rt = CRenderToTexture::getCurrent();

  tech->activate();
  mesh->activateAndRender();

}