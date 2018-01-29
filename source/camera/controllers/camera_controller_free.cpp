#include "mcv_platform.h"
#include "camera_controller_free.h"
#include "camera/camera.h"

float ICameraController::speed = 20.f;
float ICameraController::sensitivity = 0.005f;
float ICameraController::maxPitch = M_PI_2 - 1e-4;

void CCameraController_Free::update(float delta)
{
  // movement
  float deltaSpeed = speed * delta;
  VEC3 off;
  off += _camera->getFront() * EngineInput["front"].value * deltaSpeed;
  off += -_camera->getFront() * EngineInput["back"].value * deltaSpeed;
  off += -_camera->getLeft() * EngineInput["left"].value * deltaSpeed;
  off += _camera->getLeft() * EngineInput["right"].value * deltaSpeed;

  // rotation
  float yaw, pitch;
  getYawPitchFromVector(_camera->getFront(), &yaw, &pitch);
  
  auto& mouse = EngineInput.mouse();
  if(mouse.button(Input::MOUSE_RIGHT).isPressed())
  {
    VEC2 mOff = mouse._position_delta;
    yaw += -mOff.x * sensitivity;
    pitch += -mOff.y * sensitivity;
    pitch = clamp(pitch, -maxPitch, maxPitch);
  }

  // final values
  VEC3 newPos = _camera->getPosition() + off;
  VEC3 newFront = getVectorFromYawPitch(yaw, pitch);

  _camera->lookAt(newPos, newPos + newFront);
}
