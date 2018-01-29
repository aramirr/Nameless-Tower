#pragma once

#include "camera/camera_controller.h"

class CCameraController_Free : public ICameraController
{
public:
  void update(float delta) override;

private:
};