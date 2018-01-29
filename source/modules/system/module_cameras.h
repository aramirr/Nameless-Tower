#pragma once

#include "modules/module.h"

class CModuleCameras : public IModule
{
  


public:
  CModuleCameras(const std::string& name);
  bool start() override;
  bool stop() override;
  void update(float delta) override;
	void render() override;

  void blendCamera(CCamera* camera, float blendTime = 0.f);

private:
	void renderInMenu();

  struct TMixedCamera
  {
    CCamera* camera;
    float blendTime; // gets to full ratio (1.f) in 2.f seconds
    float ratio;  // blend ratio
    float time; // current blending time
  };

  CCamera blendCameras(const CCamera* camera1, const CCamera* camera2, float ratio) const;

  std::vector<TMixedCamera> _mixedCameras;
};
