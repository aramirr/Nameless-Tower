#include "mcv_platform.h"
#include "module_test_axis.h"
#include "camera/camera.h"
#include "render/render_objects.h"
#include "entity/entity.h"
#include "modules/module_entities.h"
#include "resources/resources_manager.h"
#include "render/texture/texture.h"
#include "render/texture/material.h"
#include "handle/handle.h"
#include "components/comp_name.h"
#include "components/comp_transform.h"
#include "components/comp_camera.h"
#include "entity/entity_parser.h"
#include "render/render_manager.h"


bool CModuleTestAxis::start()
{

  json jboot = loadJson("data/boot.json");

  // Auto load some scenes
  std::vector< std::string > scenes_to_auto_load = jboot["boot_scenes"];
  for (auto& scene_name : scenes_to_auto_load) {
    dbg("Autoloading scene %s\n", scene_name.c_str());
    TEntityParseContext ctx;
    parseScene(scene_name, ctx);
  }

  // -------------------------------------------
  if (!cb_camera.create(CB_CAMERA))
    return false;
  // -------------------------------------------
  if (!cb_object.create(CB_OBJECT))
    return false;
  // -------------------------------------------
  if (!cb_light.create(CB_LIGHT))
    return false;
  // -------------------------------------------
  if (!cb_globals.create(CB_GLOBALS))
    return false;
  cb_globals.global_exposure_adjustment = 1.f;
  cb_globals.global_ambient_adjustment = 1.f;
  cb_globals.global_world_time = 0.f;
  cb_globals.global_hdr_enabled = 1.f;
  cb_globals.global_gamma_correction_enabled = 1.f;
  cb_globals.global_tone_mapping_mode = 1.f;
  // -------------------------------------------
  if (!cb_gui.create(CB_GUI))
    return false;

  cb_light.activate();
  cb_object.activate();
  cb_camera.activate();
  cb_globals.activate();
  cb_gui.activate();

  return true;
}

bool CModuleTestAxis::stop()
{
  cb_light.destroy();
  cb_camera.destroy();
  cb_object.destroy();
  cb_globals.destroy();
  return true;
}

void CModuleTestAxis::update(float delta)
{
  /*
  static int nitems = 10;
  ImGui::DragInt("NumItems", &nitems, 0.2f, 1, 100);
  static float items_scale = 20.0f;
  ImGui::DragFloat("Scale", &items_scale, 0.1f, 1, 50);
  if (ImGui::SmallButton("Create Grid Of Load")) {
    for (int nz = -nitems; nz <= nitems; ++nz) {
      for (int nx = -nitems; nx <= nitems; ++nx) {
        TEntityParseContext ctx;
        float ux = (float)nx / (float)nitems;   // -1 ... 1
        float uz = (float)nz / (float)nitems;
        ctx.root_transform.setPosition(VEC3(ux, 0.f, uz) *items_scale);
        parseScene("data/prefabs/test_load.prefab", ctx);
      }
    }
  }
  */


}


void CModuleTestAxis::render()
{

  // Render the grid
  cb_object.obj_world = MAT44::Identity;
  cb_object.obj_color = VEC4(1,1,1,1);
  cb_object.updateGPU();

  auto solid = Resources.get("data/materials/solid.material")->as<CMaterial>();
  solid->activate();

  auto grid = Resources.get("grid.mesh")->as<CRenderMesh>();
  grid->activateAndRender();
  auto axis = Resources.get("axis.mesh")->as<CRenderMesh>();
  axis->activateAndRender();

}