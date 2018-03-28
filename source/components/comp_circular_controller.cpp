#include "mcv_platform.h"
#include "entity/entity_parser.h"
#include "comp_circular_controller.h"
#include "comp_transform.h"

DECL_OBJ_MANAGER("circular_controller", TCompCircularController);

void TCompCircularController::debugInMenu() {
  ImGui::DragFloat("Speed", &speed, 0.1f, 0.f, 20.f);
  ImGui::Text("Yaw %f", rad2deg(curr_yaw));
}


void TCompCircularController::load(const json& j, TEntityParseContext& ctx) {
  speed = j.value("speed", 1.0f);

}

void TCompCircularController::update(float dt) {

  curr_yaw += speed * dt;

  TCompTransform *c_my_transform = get<TCompTransform>();
  
  float X = 0.f;
  float Y = 0.f;
  float Z = 0.f;
  VEC3 up = c_my_transform->getRotation();

  if (up.x > up.y && up.x > up.z)X += 1.f;
  if (up.y > up.x && up.y > up.z)Y += 1.f;
  if (up.z > up.x && up.z > up.y)Z += 1.f;

  float y, p, r;
  c_my_transform->getYawPitchRoll(&y, &p, &r);
  c_my_transform->setYawPitchRoll(curr_yaw * up.y * Y, curr_yaw * up.z * Z, curr_yaw * up.x * X);

  std::string str = std::to_string(up.x) + " - " + std::to_string(up.y) + " - " + std::to_string(up.z);

  dbg("------------------------------------------------------------------------------\n");
  dbg(str.c_str());
  dbg("\n");

  assert(c_my_transform);
}

