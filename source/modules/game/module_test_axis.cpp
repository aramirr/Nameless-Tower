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
#include "components/juan/comp_name.h"
#include "components/juan/comp_transform.h"
#include "components/camera/comp_camera.h"
#include "components/camera/comp_camera_manager.h"
#include "entity/entity_parser.h"
#include "render/render_manager.h"

CCamera        camera;

bool CModuleTestAxis::start()
{

  {
    TEntityParseContext ctx;
    parseScene("data/scenes/scene_basic.scene", ctx);
  }
  {
	  TEntityParseContext ctx;
	  parseScene("data/scenes/Skybox01.scene", ctx);
  }
	{
		TEntityParseContext ctx;

		parseScene("data/scenes/TorreMilestoneFinal2.scene", ctx);
	}
	{
		TEntityParseContext ctx;

		parseScene("data/scenes/BossInt.scene", ctx);
	}

	/*
	json jboot = loadJson("data/boot.json");

  // Auto load some scenes
  std::vector< std::string > scenes_to_auto_load = jboot["boot_scenes"];
  for (auto& scene_name : scenes_to_auto_load) {
    dbg("Autoloading scene %s\n", scene_name.c_str());
    TEntityParseContext ctx;
    parseScene(scene_name, ctx);

  }

  }
	*/
  camera.lookAt(VEC3(12.0f, 8.0f, 8.0f), VEC3::Zero, VEC3::UnitY);
  camera.setPerspective(60.0f * 180.f / (float)M_PI, 0.1f, 1000.f);

  return true;
}

bool CModuleTestAxis::stop()
{
	Engine.getEntities().destroyAllEntities();
	Engine.getCameras().destroyAllCameras();
  return true;
}

void CModuleTestAxis::update(float delta)
{

	if (carga) {
		CEntity* cam = (CEntity*)getEntityByName("camera_manager");

		TCompCameraManager* cm = cam->get<TCompCameraManager>();
		assert(cm);

		cm->activateCinematic("prueba");

		carga = false;
	}


  static VEC3 world_pos;
  ImGui::DragFloat3("Pos", &world_pos.x, 0.025f, -50.f, 50.f);

  VEC2 mouse = EngineInput.mouse()._position;
  if (h_e_camera.isValid()) {
    CEntity* e_camera = h_e_camera;
    TCompCamera* c_camera = e_camera->get< TCompCamera >();

    VEC3 screen_coords;
    bool inside = c_camera->getScreenCoordsOfWorldCoord(world_pos, &screen_coords);
    ImGui::Text("Inside: %s  Coords: %1.2f, %1.2f  Z:%f", inside ? "YES" : "NO ", screen_coords.x, screen_coords.y, screen_coords.z);
    ImGui::Text("Mouse at %1.2f, %1.2f", mouse.x, mouse.y);
  }

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



}


void CModuleTestAxis::render()
{

  // Find the entity with name 'the_camera'
  h_e_camera = getEntityByName("the_camera");
  if (h_e_camera.isValid()) {
    CEntity* e_camera = h_e_camera;
    TCompCamera* c_camera = e_camera->get< TCompCamera >();
    assert(c_camera);
    activateCamera(*c_camera);
    CRenderManager::get().setEntityCamera(h_e_camera);
  }
  else {
    activateCamera(camera);
  }

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

		parseScene("data/scenes/lights.scene", ctx);
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

  cb_light.activate();
  cb_object.activate();

  cb_camera.activate();

  return true;
}

bool CModuleTestAxis::stop()
{
  cb_light.destroy();
  cb_camera.destroy();
  cb_object.destroy();
  return true;
}

void CModuleTestAxis::update(float delta)
{

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