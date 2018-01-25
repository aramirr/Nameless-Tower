#include "mcv_platform.h"
#include "module_entities.h"
#include "render/render_objects.h"

std::vector< TEntity* > entities;

void loadEntities(const char* filename) {

  // Load json with entities array
  json j_entities = loadJson(filename);

  // Create a new entity for each item in the json array
  // loaded from the file
  for (auto& x : json::iterator_wrapper(j_entities)) {
    TEntity *e = new TEntity();
    e->load(x.value());
    entities.push_back(e);
  }
}

TEntity* getEntityByName(const char* name) {
  for (auto& e : entities) {
    if (e->name == name)
      return e;
  }
  return nullptr;
}


void CModuleEntities::update(float delta)
{

}

void CModuleEntities::render()
{

  if (ImGui::TreeNode("Entities")) {

    // Just to do some tests of the IsInFront/left/etc
    TEntity* e0 = nullptr;

    for (auto& e : entities) {
      ImGui::PushID(e);
      if (ImGui::TreeNode(e->name.c_str())) {
        auto& t = e->transform;
        t.debugInMenu();
        // All entities except the first one, check if are in the front/left of the first entity
        if (e0) {
          bool is_e0_in_front_of_me = t.isInFront(e0->transform.getPosition());
          bool is_e0_in_left_of_me = t.isInLeft(e0->transform.getPosition());
          ImGui::LabelText("In Front", "%s", is_e0_in_front_of_me ? "YES" : "NO");
          ImGui::LabelText("In Left", "%s", is_e0_in_left_of_me ? "YES" : "NO");
          float delta_yaw_to_e0 = t.getDeltaYawToAimTo(e0->transform.getPosition());
          ImGui::LabelText("Delta To e0", "%f", rad2deg(delta_yaw_to_e0));

          if (ImGui::Button("Aim To..")) {
            float old_yaw, old_pitch;
            t.getYawPitchRoll(&old_yaw, &old_pitch);
            float new_yaw = old_yaw + delta_yaw_to_e0 * 0.15f;
            t.setYawPitchRoll(new_yaw, old_pitch);
          }
          static float fov = 90.f;
          ImGui::SameLine();
          bool in_in_fov = t.isInFov(e0->transform.getPosition(), deg2rad(fov));
          ImGui::Text("Inside:%d", in_in_fov);
          ImGui::DragFloat("Fov", &fov, 1.0f, 1.f, 120.f);
        }

        ImGui::TreePop();
      }

      if (!e0)
        e0 = e;

      ImGui::PopID();
    }
    ImGui::TreePop();
  }

  // Do the basic render
  for (auto& e : entities) {
    cb_object.obj_world = e->transform.asMatrix();
    //cb_object.obj_color = e->color
    cb_object.updateGPU();
    e->tech->activate();
    e->mesh->activateAndRender();
  }

}
