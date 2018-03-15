#include "mcv_platform.h"
#include "comp_light_dir.h"
#include "components/juan/comp_transform.h"
#include "render/render_objects.h"    // cb_light
#include "render/texture/texture.h" 
#include "render/texture/texture_slots.h" 

DECL_OBJ_MANAGER("light_dir", TCompLightDir);

// -------------------------------------------------
void TCompLightDir::debugInMenu() {
  TCompCamera::debugInMenu();
}

// -------------------------------------------------
void TCompLightDir::renderDebug() {
  TCompCamera::renderDebug();
  ImGui::DragFloat("Intensity", &intensity, 0.01f, 0.f, 10.f);
  ImGui::ColorEdit3("Color", &color.x);
}

// -------------------------------------------------
void TCompLightDir::load(const json& j, TEntityParseContext& ctx) {
  TCompCamera::load( j, ctx );
  if(j.count("color"))
    color = loadVEC4(j["color"]);
  intensity = j.value("intensity", intensity);

  if( j.count("projector")) {
    std::string projector_name = j.value("projector", "");
    projector = Resources.get(projector_name)->as<CTexture>();
  }

}

void TCompLightDir::update(float dt) {
  // Can't use the TCompCamera::update because inside it calls 
  // get<TCompTransform> which tries to convert 'this' to an instance
  // of TCompCamera, but will fail because we are a CompLightDir
  TCompTransform* c = get<TCompTransform>();
  if (!c)
    return;
  this->lookAt(c->getPosition(), c->getPosition() + c->getFront(), c->getUp());
}

// -------------------------------------------------
// Updates the Shader Cte Light with MY information
void TCompLightDir::activate() {
  TCompTransform* c = get<TCompTransform>();
  if (!c)
    return;

  if (projector)
    projector->activate( TS_LIGHT_PROJECTOR );

  // To avoid converting the range -1..1 to 0..1 in the shader
  // we concatenate the view_proj with a matrix to apply this offset
  MAT44 mtx_offset = MAT44::CreateScale(VEC3(0.5f, -0.5f, 1.0f))
                   * MAT44::CreateTranslation(VEC3(0.5f, 0.5f, 0.0f));

  cb_light.light_color = color;
  cb_light.light_intensity = intensity;
  cb_light.light_pos = c->getPosition();
  cb_light.light_view_proj_offset = getViewProjection() * mtx_offset;
  cb_light.updateGPU();
}


