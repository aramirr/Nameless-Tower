#pragma once

#include "modules/module.h"
#include "geometry/interpolators.h"

class CModuleCameras : public IModule
{
public:
  enum EPriority { DEFAULT = 0, GAMEPLAY, TEMPORARY, DEBUG, NUM_PRIORITIES };

  CModuleCameras(const std::string& name);
  bool start() override;
  bool stop() override;
  void update(float delta) override;
	void render() override;

  void setDefaultCamera(CCamera* camera);
  void blendInCamera(CCamera* camera, float blendTime = 0.f, const std::string& name = "", EPriority priority = EPriority::DEFAULT, Interpolator::IInterpolator* interpolator = nullptr);
  void blendOutCamera(CCamera* camera, float blendTime = 0.f);

private:
	void renderInMenu();

  struct TMixedCamera
  {
    enum EState {ST_BLENDING_IN, ST_IDLE, ST_BLENDING_OUT};

    CCamera* camera;
    std::string name;
    EState state = EState::ST_IDLE;
    EPriority type = EPriority::DEFAULT;
    float blendInTime = 0.f; // gets to full ratio (1.f) in 2.f seconds
    float blendOutTime = 0.f; // gets to full ratio (1.f) in 2.f seconds
    float ratio = 0.f;  // blend ratio
    float time = 0.f; // current blending time
    Interpolator::IInterpolator* interpolator = nullptr;

    void blendIn(float duration);
    void blendOut(float duration);
  };

  using VMixedCameras = std::vector<TMixedCamera>;

  TMixedCamera* getMixedCamera(CCamera* camera);
  CCamera blendCameras(const CCamera* camera1, const CCamera* camera2, float ratio) const;

  VMixedCameras _mixedCameras;
  CCamera* _defaultCamera = nullptr;
};
