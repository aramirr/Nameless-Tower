#include "mcv_platform.h"
#include "comp_render_blur_radial.h"
#include "resources/resources_manager.h"
#include "render/blur_step.h"
#include "render/render_objects.h"

DECL_OBJ_MANAGER("render_blur_radial", TCompRenderBlurRadial);

// ---------------------
void TCompRenderBlurRadial::debugInMenu() {
  ImGui::Checkbox("Enabled", &enabled);
  ImGui::InputFloat("Weights Center", &weights.x);
  ImGui::InputFloat("Weights 1st", &weights.y);
  ImGui::InputFloat("Weights 2nd", &weights.z);
  ImGui::InputFloat("Weights 3rd", &weights.w);
  if (ImGui::SmallButton("box")) {
    distance_factors = VEC4(1, 2, 3, 4);
    weights = VEC4(1, 1, 1, 1);
  }
  ImGui::SameLine();
  if (ImGui::SmallButton("gauss")) {
    weights = VEC4(70, 56, 28, 8);
    distance_factors = VEC4(1, 2, 3, 4);
  }
  ImGui::SameLine();
  if (ImGui::SmallButton("linear")) {
    // This is a 5 taps kernel (center + 2 taps on each side)
    // http://rastergrid.com/blog/2010/09/efficient-gaussian-blur-with-linear-sampling/
    weights = VEC4(0.2270270270f, 0.3162162162f, 0.0702702703f, 0.f);
    distance_factors = VEC4(1.3846153846f, 3.2307692308f, 0.f, 0.f);
  }
  if (ImGui::SmallButton("Preset1")) {
    weights = VEC4(70, 56, 28, 8);
    distance_factors = VEC4(1, 2, 3, 4);
    global_distance = 2.7f;
    //nactive_steps = 3;
  }
  if (ImGui::SmallButton("Preset2")) {
    weights = VEC4(70, 56, 28, 8);
    distance_factors = VEC4(1, 2, 3, 4);
    global_distance = 2.0f;
    //nactive_steps = 2;
  }
  ImGui::DragFloat("global_distance", &global_distance, 0.01f, 0.1f, 8.0f);
  ImGui::InputFloat("Distance 2nd Tap", &distance_factors.x);
  ImGui::InputFloat("Distance 3rd Tap", &distance_factors.y);
  ImGui::InputFloat("Distance 4th Tap", &distance_factors.z);
  if (ImGui::SmallButton("Default Distances")) {
    distance_factors = VEC4(1, 2, 3, 4);
  }
}

void TCompRenderBlurRadial::load(const json& j, TEntityParseContext& ctx) {
  enabled = j.value( "enabled", true);
  global_distance = j.value("global_distance", 1.0f);
  distance_factors = VEC4(1, 2, 3, 4);

  weights.x = j.value("w0", 1.f);
  weights.y = j.value("w1", 1.f);
  weights.z = j.value("w2", 1.f);
  weights.w = j.value("w3", 1.f);
  
  if (j.value("box_filter", false))
    weights = VEC4(1, 1, 1, 1);
  else if (j.value("gauss_filter", false))
    weights = VEC4(70, 56, 28, 8);
    /*
                  1
                1   1
              1   2   1
            1   3   3   1
          1   4   6   4   1
        1   5   10 10   5   1
      1   6   15  20  15  6   1
    1   7   21  35  35  21  7   1
  1   8   28  56  70  56  28  8   1   <-- Four taps, discard the last 1
  */

  int xres = Render.width;
  int yres = Render.height;

  static int g_blur_radial_counter = 0;

  rt_output = new CRenderToTexture();
  char rt_name[64];
  sprintf(rt_name, "BlurRadial_%02d", g_blur_radial_counter++);
  bool is_ok = rt_output->createRT(rt_name, xres, yres, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_UNKNOWN);
  assert(is_ok);

  tech = Resources.get("blur_radial.tech")->as<CRenderTechnique>();
  mesh = Resources.get("unit_quad_xy.mesh")->as<CRenderMesh>();
}

CTexture* TCompRenderBlurRadial::apply( CTexture* in_texture) {
  if (!enabled)
    return in_texture;
  CTraceScoped scope("CompBlur");

  rt_output->activateRT();
  in_texture->activate(TS_ALBEDO);

  // Sum( Weights ) = 1 to not loose energy. +2 is to account for left and right taps
  float normalization_factor =
      1 * weights.x
    + 2 * weights.y
    + 2 * weights.z
    + 2 * weights.w
    ;
  cb_blur.blur_w.x = weights.x / normalization_factor;
  cb_blur.blur_w.y = weights.y / normalization_factor;
  cb_blur.blur_w.z = weights.z / normalization_factor;
  cb_blur.blur_w.w = weights.w / normalization_factor;
  cb_blur.blur_d.x = distance_factors.x;
  cb_blur.blur_d.y = distance_factors.y;
  cb_blur.blur_d.z = distance_factors.z;
  cb_blur.blur_d.w = distance_factors.w;  // Not used
  cb_blur.blur_distance.x = global_distance;

  cb_blur.activate();
  cb_blur.updateGPU();

  tech->activate();
  mesh->activateAndRender();

  return rt_output;
}