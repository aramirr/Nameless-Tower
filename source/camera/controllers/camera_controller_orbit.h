#pragma once

#include "camera/camera_controller.h"

class CCameraController_Orbit : public ICameraController
{
public:
  void update(float delta) override;
  void setDistance(float distance) { _distance = distance; }
  void setTarget(VEC3 point) { _target = point; }

private:
  float _distance;
  VEC3 _target;
};