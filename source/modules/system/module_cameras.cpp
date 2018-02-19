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
  if (_defaultCamera)
  {
    resultCamera = *_defaultCamera;
  }

  for (int i = 0; i < NUM_PRIORITIES; ++i)
  {
    for (auto& mc : _mixedCameras)
    {
      if (mc.type != i)
        continue;

      mc.time += delta;
      if (mc.state == TMixedCamera::ST_BLENDING_IN)
      {
        mc.ratio = clamp(mc.time / mc.blendInTime, 0.f, 1.f);
        if (mc.ratio >= 1.f)
        {
          mc.state = TMixedCamera::ST_IDLE;
          mc.time = 0.f;
        }
      }
      else if (mc.state == TMixedCamera::ST_BLENDING_OUT)
      {
        mc.ratio = 1.f - clamp(mc.time / mc.blendOutTime, 0.f, 1.f);
      }

      if (mc.ratio > 0.f)
      {
        float ratio = mc.ratio;
        if (mc.interpolator)
          ratio = mc.interpolator->blend(0.f, 1.f, ratio);

        resultCamera = blendCameras(&resultCamera, mc.camera, ratio);
      }
    }
  }

  // remove old ones
  VMixedCameras::iterator it = _mixedCameras.begin();
  while (it != _mixedCameras.end())
  {
    if (it->ratio <= 0.f)
      it = _mixedCameras.erase(it);
    else
      ++it;
  }

  camera = resultCamera;
}

void CModuleCameras::render()
{
  renderInMenu();
}

void renderInterpolator(const char* name, Interpolator::IInterpolator& interpolator)
{
  const int nsamples = 50;
  float values[nsamples];
  for (int i = 0; i < nsamples; ++i)
  {
    values[i] = interpolator.blend(0.f, 1.f, (float)i / (float)nsamples);
  }
  ImGui::PlotLines(name, values, nsamples, 0, 0,
    std::numeric_limits<float>::min(), std::numeric_limits<float>::max(),
    ImVec2(150, 80));
}

void CModuleCameras::renderInMenu()
{
	if (ImGui::TreeNode("Cameras"))
	{
    for (auto& mc : _mixedCameras)
    {
      ImGui::Text(mc.name.c_str());
      ImGui::SameLine();
      ImGui::Text("TYPE: %d", mc.type);
      ImGui::SameLine();
      if(mc.state == TMixedCamera::ST_BLENDING_IN)
        ImGui::Text("Blending In");
      else if (mc.state == TMixedCamera::ST_BLENDING_OUT)
        ImGui::Text("Blending Out");
      else
        ImGui::Text("Idle");
      ImGui::SameLine();
      ImGui::DragFloat("Time", &mc.time);
      ImGui::SameLine();
      ImGui::ProgressBar(mc.ratio);
    }
		ImGui::TreePop();
	}

  if (ImGui::TreeNode("Interpolators"))
  {
    renderInterpolator("Linear", Interpolator::TLinearInterpolator());
    renderInterpolator("Quad in", Interpolator::TQuadInInterpolator());
    renderInterpolator("Quad out", Interpolator::TQuadOutInterpolator());
    renderInterpolator("Quad in out", Interpolator::TQuadInOutInterpolator());
    renderInterpolator("Cubic in", Interpolator::TCubicInInterpolator());
    renderInterpolator("Cubic out", Interpolator::TCubicOutInterpolator());
    renderInterpolator("Cubic in out", Interpolator::TCubicInOutInterpolator());
    renderInterpolator("Quart in", Interpolator::TQuartInInterpolator());
    renderInterpolator("Quart out", Interpolator::TQuartOutInterpolator());
    renderInterpolator("Quart in out", Interpolator::TQuartInOutInterpolator());
    renderInterpolator("Quint in", Interpolator::TQuintInInterpolator());
    renderInterpolator("Quint out", Interpolator::TQuintOutInterpolator());
    renderInterpolator("Quint in out", Interpolator::TQuintInOutInterpolator());
    renderInterpolator("Back in", Interpolator::TBackInInterpolator());
    renderInterpolator("Back out", Interpolator::TBackOutInterpolator());
    renderInterpolator("Back in out", Interpolator::TBackInOutInterpolator());
    renderInterpolator("Elastic in", Interpolator::TElasticInInterpolator());
    renderInterpolator("Elastic out", Interpolator::TElasticOutInterpolator());
    renderInterpolator("Elastic in out", Interpolator::TElasticInOutInterpolator());
    renderInterpolator("Bounce in", Interpolator::TBounceInInterpolator());
    renderInterpolator("Bounce out", Interpolator::TBounceOutInterpolator());
    renderInterpolator("Bounce in out", Interpolator::TBounceInOutInterpolator());
    renderInterpolator("Circular in", Interpolator::TCircularInInterpolator());
    renderInterpolator("Circular out", Interpolator::TCircularOutInterpolator());
    renderInterpolator("Circular in out", Interpolator::TCircularInOutInterpolator());
    renderInterpolator("Expo in", Interpolator::TExpoInInterpolator());
    renderInterpolator("Expo out", Interpolator::TExpoOutInterpolator());
    renderInterpolator("Expo in out", Interpolator::TExpoInOutInterpolator());
    renderInterpolator("Sine in", Interpolator::TSineInInterpolator());
    renderInterpolator("Sine out", Interpolator::TSineOutInterpolator());
    renderInterpolator("Sine in out", Interpolator::TSineInOutInterpolator());

    ImGui::TreePop();
  }
}

void CModuleCameras::setDefaultCamera(CCamera* camera)
{
  _defaultCamera = camera;
}

void CModuleCameras::blendInCamera(CCamera* camera, float blendTime, const std::string& name, EPriority priority, Interpolator::IInterpolator* interpolator)
{
  TMixedCamera* mc = getMixedCamera(camera);
  if (mc)
  {
    return;
  }

  if (priority == EPriority::GAMEPLAY)
  {
    for (auto& mc : _mixedCameras)
    {
      if (mc.type == priority)
      {
        mc.blendOut(blendTime);
      }
    }
  }

  TMixedCamera new_mc;
  new_mc.camera = camera;
  new_mc.name = name;
  new_mc.type = priority;
  new_mc.blendIn(blendTime);
  new_mc.interpolator = interpolator;

  _mixedCameras.push_back(new_mc);
}

void CModuleCameras::blendOutCamera(CCamera* camera, float blendTime)
{
  TMixedCamera* mc = getMixedCamera(camera);
  if (mc)
  {
    mc->blendOut(blendTime);
  }
}

CModuleCameras::TMixedCamera* CModuleCameras::getMixedCamera(CCamera* camera)
{
  for (auto& mc : _mixedCameras)
  {
    if (mc.camera == camera)
    {
      return &mc;
    }
  }
  return nullptr;
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
    VEC3 newFront = VEC3::Lerp(camera1->getFront(), camera2->getFront(), ratio);
    camera.lookAt(newPos, newPos + newFront);
    return camera;
  }
}


void CModuleCameras::TMixedCamera::blendIn(float duration)
{
  blendInTime = duration;
  state = blendInTime == 0.f ? ST_IDLE : ST_BLENDING_IN;
  time = 0.f;
}

void CModuleCameras::TMixedCamera::blendOut(float duration)
{
  blendOutTime = duration;
  state = ST_BLENDING_OUT;
  time = 0.f;
}
