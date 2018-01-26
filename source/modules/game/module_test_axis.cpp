#include "mcv_platform.h"
#include "module_test_axis.h"
#include "camera/camera.h"
#include "render/render_objects.h"
#include "entity/entity.h"
#include "modules/module_entities.h"
#include "resources/resources_manager.h"
#include "render/texture/texture.h"

extern CRenderTechnique tech_solid;

CCamera camera;

bool CModuleTestAxis::start()
{
  // One time at boot
  Resources.registerResourceClass(getResourceClassOf<CTexture>());
  Resources.registerResourceClass(getResourceClassOf<CRenderMesh>());

  loadEntities("data/entities.json");

  camera.lookAt(VEC3(12.0f, 8.0f, 8.0f), VEC3::Zero, VEC3::UnitY);
  camera.setPerspective(60.0f * 180.f / (float)M_PI, 0.1f, 1000.f);

  // -------------------------------------------
  if (!cb_camera.create(CB_CAMERA))
    return false;
  // -------------------------------------------
  if (!cb_object.create(CB_OBJECT))
    return false;

  cb_object.activate();
  cb_camera.activate();

  return true;
}

bool CModuleTestAxis::stop()
{
  cb_camera.destroy();
  cb_object.destroy();
  return true;
}

void CModuleTestAxis::update(float delta)
{
  if (ImGui::TreeNode("Camera")) {
    VEC3 new_pos = camera.getPosition();
    VEC3 new_target = camera.getTarget();
    bool changed = false;
    changed |= ImGui::DragFloat3("Pos", &new_pos.x, 0.025f, -50.f, 50.f);
    changed |= ImGui::DragFloat3("Target", &new_target.x, 0.025f, -50.f, 50.f);
    if( changed ) 
      camera.lookAt(new_pos, new_target);
    ImGui::TreePop();
  }

}

void CModuleTestAxis::render()
{
  tech_solid.activate();

  activateCamera(camera);

  // Render the grid
  cb_object.obj_world = MAT44::Identity;
  cb_object.obj_color = VEC4(1,1,1,1);
  cb_object.updateGPU();

  auto grid = Resources.get("grid.mesh")->as<CRenderMesh>();
  grid->activateAndRender();
  auto axis = Resources.get("axis.mesh")->as<CRenderMesh>();
  axis->activateAndRender();

}