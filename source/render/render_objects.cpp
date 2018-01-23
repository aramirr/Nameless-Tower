#include "mcv_platform.h"
#include "render_objects.h"

CRenderCte<CCteCamera> cb_camera;
CRenderCte<CCteObject> cb_object;

struct TVtxPosClr {
  VEC3 pos;
  VEC4 color;
  TVtxPosClr(VEC3 new_pos, VEC4 new_color) : pos(new_pos), color(new_color){}
};

CRenderMesh* axis;
CRenderMesh* grid;

// ---------------------------------------------------
CRenderMesh* createAxis() {
  CRenderMesh* mesh = new CRenderMesh;
  // Axis aligned X,Y,Z of sizes 1,2,3
  float vertices[] =
  {
    0.0f, 0.0f, 0.0f,  1, 0, 0, 1,
    1.0f, 0.0f, 0.0f,  1, 0, 0, 1,
    0.0f, 0.0f, 0.0f,  0, 1, 0, 1,
    0.0f, 2.0f, 0.0f,  0, 1, 0, 1,
    0.0f, 0.0f, 0.0f,  0, 0, 1, 1,
    0.0f, 0.0f, 3.0f,  0, 0, 1, 1,
  };
  if (!mesh->create(vertices, sizeof(vertices), "PosClr", CRenderMesh::LINE_LIST))
    return nullptr;
  return mesh;
}

// ---------------------------------------------------
CRenderMesh* createGridXZ( int nsteps ) {
  CRenderMesh* mesh = new CRenderMesh;

  std::vector<TVtxPosClr> vtxs;
  VEC4 clr2(0.5f, 0.5f, 0.5f, 1.0f);
  VEC4 clr1(0.25f, 0.25f, 0.25f, 1.0f);
  for (int i = -nsteps; i <= nsteps; ++i) {
    VEC4 clr = (i % 5) ? clr1 : clr2;
    vtxs.emplace_back(VEC3(i, 0, nsteps), clr);
    vtxs.emplace_back(VEC3(i, 0, -nsteps), clr);
    vtxs.emplace_back(VEC3(nsteps, 0, i), clr);
    vtxs.emplace_back(VEC3(-nsteps, 0, i), clr);
  }

  if (!mesh->create(vtxs.data(), vtxs.size() * sizeof(TVtxPosClr), "PosClr", CRenderMesh::LINE_LIST))
    return nullptr;
  return mesh;
}


bool createRenderObjects() {

  axis = createAxis();
  grid = createGridXZ(20);

  return true;
}

void destroyRenderObjects() {
  if (axis) {
    axis->destroy();
    delete axis;
    axis = nullptr;
  }
  if (grid) {
    grid->destroy();
    delete grid;
    grid = nullptr;
  }
}

void activateCamera(const CCamera& camera) {
  cb_camera.camera_view = camera.getView();
  cb_camera.camera_proj = camera.getProjection();
  cb_camera.camera_pos = camera.getPosition();
  cb_camera.updateGPU();
}

