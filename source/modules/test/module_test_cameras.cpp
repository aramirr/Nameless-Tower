#include "mcv_platform.h"
#include "module_test_cameras.h"

extern CCamera camera;

bool CModuleTestCameras::start()
{
  _free.setCamera(&camera);
  
  _orbit.setCamera(&camera);
  _orbit.setDistance(10.f);
  _orbit.setTarget(VEC3::Zero);

  _cameraA = camera;
  _cameraA.lookAt(VEC3(5, 5, 5), VEC3::Zero);

  _cameraB = camera;
  _cameraB.lookAt(VEC3(5, 5, -5), VEC3::Zero);

  _cameraC = camera;
  _cameraC.lookAt(VEC3(-5, 5, 0), VEC3::Zero);

  return true;
}

void CModuleTestCameras::update(float delta)
{
  //_free.update(delta);
  _orbit.update(delta);

  if (EngineInput['1'].getsPressed())
  {
    Engine.getCameras().blendCamera(&_cameraA, 0.f);
  }
  if (EngineInput['2'].getsPressed())
  {
    Engine.getCameras().blendCamera(&_cameraB, 1.f);
  }
  if (EngineInput['3'].getsPressed())
  {
    Engine.getCameras().blendCamera(&_cameraC, 1.f);
  }

}
