#include "mcv_platform.h"
#include "comp_camera_flyover.h"
#include "components/juan/comp_transform.h"

DECL_OBJ_MANAGER("camera_flyover", TCompCameraFlyover);

void TCompCameraFlyover::debugInMenu() {
  ImGui::DragFloat("Max Speed", &_speed, 10.f, 1.f, 100.f);
  ImGui::DragFloat("Sensitivity", &_sensitivity, 0.01f, 0.001f, 0.1f);
  ImGui::DragFloat("Inertia", &_ispeed_reduction_factor, 0.001f, 0.7f, 1.f);
  ImGui::LabelText("Curr Speed", "%f", _ispeed);
}

void TCompCameraFlyover::load(const json& j, TEntityParseContext& ctx) {
  _speed = j.value("speed", _speed);
  _sensitivity = j.value("sensitivity", _sensitivity);
  _ispeed_reduction_factor = j.value("speed_reduction_factor", _ispeed_reduction_factor);
}

void TCompCameraFlyover::update(float scaled_dt)
{
  TCompTransform* c_transform = get<TCompTransform>();
  if (!c_transform)
    return;
  
  // if the game is paused, we still want full camera speed 
  float dt = CEngine::get().getUnscaledDeltaTime();

  VEC3 pos = c_transform->getPosition();
  VEC3 front = c_transform->getFront();
  VEC3 left = c_transform->getLeft();
  VEC3 up = VEC3::UnitY;

  _ispeed *= _ispeed_reduction_factor;

  // movement
  float deltaSpeed = _speed * dt;
  if (EngineInput["turbo"])
    deltaSpeed *= 4.f;

  if (EngineInput["front"].value)
    _ispeed.z = 1.f;
  if (EngineInput["back"].value)
    _ispeed.z = -1.f;
  if (EngineInput["left"].value)
    _ispeed.x = 1.f;
  if (EngineInput["right"].value)
    _ispeed.x = -1.f;
  if (EngineInput["up"].value)
    _ispeed.y = 1.f;
  if (EngineInput["down"].value)
    _ispeed.y = -1.f;

  // Amount in each direction
  VEC3 off;
  off += front * _ispeed.z * deltaSpeed;
  off += left * _ispeed.x * deltaSpeed;
  off += up * _ispeed.y * deltaSpeed;

  // rotation
  float yaw, pitch;
  getYawPitchFromVector(front, &yaw, &pitch);

  auto& mouse = EngineInput.mouse();
  if (mouse.button(Input::MOUSE_RIGHT).isPressed())
  {
    VEC2 mOff = mouse._position_delta;
    yaw += -mOff.x * _sensitivity;
    pitch += -mOff.y * _sensitivity;
    pitch = clamp(pitch, -_maxPitch, _maxPitch);
  }

  // final values
  VEC3 newPos = pos + off;
  VEC3 newFront = getVectorFromYawPitch(yaw, pitch);

  c_transform->lookAt(newPos, newPos + newFront);
}
