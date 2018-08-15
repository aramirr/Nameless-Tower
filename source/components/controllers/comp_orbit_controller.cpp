#include "mcv_platform.h"
#include "entity/entity_parser.h"
#include "comp_orbit_controller.h"
#include "../juan/comp_transform.h"

DECL_OBJ_MANAGER("orbit_controller", TCompOrbitController);

void TCompOrbitController::debugInMenu() {
  ImGui::DragFloat("Radius", &radius, 0.1f, 0.f, 20.f);
  ImGui::DragFloat("Speed", &speed, 0.1f, 0.f, 20.f);
  ImGui::Text("Yaw %f", rad2deg(curr_yaw));
  if (h_target.isValid()) {
    if (ImGui::TreeNode("Target")) {
      h_target.debugInMenu();
      ImGui::TreePop();
    }
  }
}

void TCompOrbitController::onCreate(const TMsgEntityCreated& msg) {
  dbg("Hi, I'm TCompCircularController at onCreate\n");
}

void TCompOrbitController::registerMsgs() {
  DECL_MSG(TCompOrbitController, TMsgEntityCreated, onCreate);
}


void TCompOrbitController::load(const json& j, TEntityParseContext& ctx) {
  radius = j.value("radius", 1.0f);
  speed = j.value("speed", 1.0f);
  target_name = j.value("target", "");

  h_target = ctx.findEntityByName(target_name);

}

void TCompOrbitController::update(float dt) {
  
  if (!h_target.isValid())
    return;

  curr_yaw += speed * dt;

  CEntity* e_target = h_target;
  assert(e_target);

  TCompTransform *c_target = e_target->get<TCompTransform>();
  assert(c_target);

  TCompTransform *c_my_transform = get<TCompTransform>();

  float my_y = c_my_transform->getPosition().y;
  VEC3 my_new_pos = c_target->getPosition() + getVectorFromYaw(curr_yaw) * radius;
  my_new_pos.y = my_y;

  assert(c_my_transform);
  c_my_transform->lookAt( my_new_pos, c_target->getPosition() );

}

