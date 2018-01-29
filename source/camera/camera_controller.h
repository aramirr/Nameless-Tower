#pragma once

#include "mcv_platform.h"

class CCamera;

class ICameraController
{
public:
  virtual void update(float delta) = 0;
  void setCamera(CCamera* camera) { _camera = camera; }

protected:
  CCamera* _camera;

  static float speed;
  static float sensitivity;
  static float maxPitch;
};