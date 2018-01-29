#include "mcv_platform.h"
#include "camera_controller_orbit.h"

void CCameraController_Orbit::update(float delta)
{
  // rotation
  float yaw, pitch;
  getYawPitchFromVector(_camera->getFront(), &yaw, &pitch);

  auto& mouse = EngineInput.mouse();
  if (mouse.button(Input::MOUSE_RIGHT).isPressed())
  {
    VEC2 mOff = mouse._position_delta;
    yaw += -mOff.x * sensitivity;
    pitch += -mOff.y * sensitivity;
    pitch = clamp(pitch, -maxPitch, maxPitch);
  }

  // final values
  VEC3 newFront = getVectorFromYawPitch(yaw, pitch);
  VEC3 newPos = _target - newFront * _distance;

  _camera->lookAt(newPos, _target);
}
