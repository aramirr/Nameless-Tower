#include "mcv_platform.h"
#include "module_test_cameras.h"
#include "render/render_objects.h"
#include "render/texture/material.h"

extern CCamera camera;
extern void registerMesh(CRenderMesh* new_mesh, const char* name);

struct TVtxPosClr {
  VEC3 pos;
  VEC4 color;
  TVtxPosClr(VEC3 new_pos, VEC4 new_color) : pos(new_pos), color(new_color) {}
};

// ---------------------------------------------------
CRenderMesh* createCurveMesh(const CCurve& curve, int nsteps) {
  CRenderMesh* mesh = new CRenderMesh;

  std::vector<TVtxPosClr> vtxs;
  VEC4 clr(1.f, 1.f, 1.f, 1.0f);
  for (int i = 0; i < nsteps; ++i) {
    vtxs.emplace_back(curve.evaluateAsCatmull((float)i / (float)nsteps), clr);
    vtxs.emplace_back(curve.evaluateAsCatmull((float)(i+1) / (float)nsteps), clr);
  }

  if (!mesh->create(vtxs.data(), vtxs.size() * sizeof(TVtxPosClr), "PosClr", CRenderMesh::LINE_LIST))
    return nullptr;
  return mesh;
}

bool CModuleTestCameras::start()
{
  /*_free.setCamera(&camera);
  _orbit.setCamera(&camera);*/

  _orbit.setDistance(10.f);
  _orbit.setTarget(VEC3::Zero);

  _cameraA = camera;
  _cameraA.lookAt(VEC3(5, 5, 5), VEC3::Zero);

  _cameraB = camera;
  _cameraB.lookAt(VEC3(5, 5, -5), VEC3::Zero);

  _cameraC = camera;
  _cameraC.lookAt(VEC3(-5, 5, 0), VEC3::Zero);

  _cameraD = camera;
  _cameraD.lookAt(VEC3(-5, 5, 0), VEC3::Zero);

  Engine.getCameras().setDefaultCamera(&_cameraA);

  _orbit.setCamera(&_cameraB);
  _free.setCamera(&_cameraD);

  _curve.addKnot(VEC3(-10, -3, 5));
  _curve.addKnot(VEC3(-8, 3, 5));
  _curve.addKnot(VEC3(-6, 3, 5));
  _curve.addKnot(VEC3(-4, -3, 5));
  _curve.addKnot(VEC3(-2, -3, 5));
  _curve.addKnot(VEC3(0, 3, 5));
  _curve.addKnot(VEC3(2, 3, 5));
  _curve.addKnot(VEC3(4, -3, 5));
  _curve.addKnot(VEC3(6, -3, 5));
  _curve.addKnot(VEC3(8, 3, 5));
  _curve.addKnot(VEC3(10, 3, 5));

  registerMesh(createCurveMesh(_curve, 100), "curve.mesh");

  return true;
}

void CModuleTestCameras::update(float delta)
{
  _free.update(delta);
  _orbit.update(delta);

  if (EngineInput['1'].getsPressed())
  {
    Engine.getCameras().blendInCamera(&_cameraB, 1.f, "Gameplay B", CModuleCameras::EPriority::GAMEPLAY);
  }
  if (EngineInput['2'].getsPressed())
  {
    Engine.getCameras().blendInCamera(&_cameraC, 1.f, "Gameplay C", CModuleCameras::EPriority::GAMEPLAY);
  }
  if (EngineInput['3'].getsPressed())
  {
    static Interpolator::TCubicInOutInterpolator interpolator;
    Engine.getCameras().blendInCamera(&_cameraD, 1.f, "Temporary D", CModuleCameras::EPriority::TEMPORARY, &interpolator);
  }
  if (EngineInput['4'].getsPressed())
  {
    Engine.getCameras().blendOutCamera(&_cameraD, 0.f);
  }
}

void CModuleTestCameras::render()
{
  auto solid = Resources.get("data/materials/solid.material")->as<CMaterial>();
  solid->activate();

  activateCamera(camera);

  // Render the grid
  cb_object.obj_world = MAT44::Identity;
  cb_object.obj_color = VEC4(1, 1, 1, 1);
  cb_object.updateGPU();

  auto curve = Resources.get("curve.mesh")->as<CRenderMesh>();
  curve->activateAndRender();
}
