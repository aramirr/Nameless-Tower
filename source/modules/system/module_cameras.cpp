#include "mcv_platform.h"
#include "module_cameras.h"

extern CCamera camera;

CModuleCameras::CModuleCameras(const std::string& name)
	: IModule(name)
{}

bool CModuleCameras::start()
{

  return true;
}

bool CModuleCameras::stop()
{
  return true;
}

void CModuleCameras::update(float delta)
{
  CCamera resultCamera;

  for (auto& mc : _mixedCameras)
  {
    mc.time += delta;
    if(mc.ratio < 1.f)
    {
      mc.ratio = clamp(mc.time / mc.blendTime, 0.f, 1.f);
    }

    resultCamera = blendCameras(&resultCamera, mc.camera, mc.ratio);
  }

  camera = resultCamera;
}

void CModuleCameras::render()
{
  renderInMenu();
}

void CModuleCameras::renderInMenu()
{
	if (ImGui::TreeNode("Cameras"))
	{

		ImGui::TreePop();
	}
}


void CModuleCameras::blendCamera(CCamera* camera, float blendTime)
{
  TMixedCamera mc;
  mc.camera = camera;
  mc.blendTime = blendTime;
  mc.ratio = blendTime == 0.f ? 1.f : 0.f;
  mc.time = 0.f;
  
  _mixedCameras.push_back(mc);
}

CCamera CModuleCameras::blendCameras(const CCamera* camera1, const CCamera* camera2, float ratio) const
{
  if (ratio == 0.f)
  {
    return *camera1;
  }
  else if (ratio == 1.f)
  {
    return *camera2;
  }
  else
  {
    CCamera camera = *camera1;
    VEC3 newPos = VEC3::Lerp(camera1->getPosition(), camera2->getPosition(), ratio);
    camera.lookAt(newPos, newPos + camera.getFront());
    return camera;
  }
}

