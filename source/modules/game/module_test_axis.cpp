#include "mcv_platform.h"
#include "module_test_axis.h"
#include "camera/camera.h"
#include "geometry/angular.h"
#include "render/render_objects.h"
#include "render/mesh/mesh_loader.h"

// pos_loc * world * view * proj = pos_proj
// / wp => pos_homo

struct TTransform {
  VEC3 pos;
  //float scale;
  //QUAT rot;
  float yaw = 0.f;
  float pitch = 0.f;

  MAT44 getWorld() {
    MAT44 t = MAT44::CreateTranslation(pos);
    MAT44 r = MAT44::CreateFromYawPitchRoll(yaw, pitch, 0.0f);
    return r * t;
  }
  VEC3 getFront() const {
    return getVectorFromYaw(yaw);
  }
  VEC3 getLeft() const {
    return getVectorFromYaw(yaw + (float)M_PI_2 );
  }

  bool isInFront( VEC3 p ) const {
    VEC3 delta = p - pos;
    return delta.Dot(getFront()) > 0.f;
  }

  bool isInLeft(VEC3 p) const {
    VEC3 delta = p - pos;
    return delta.Dot(getLeft()) > 0.f;
  }

 
};



struct CRenderTechnique {
  CVertexShader *vs = nullptr;
  CPixelShader *ps = nullptr;
  void activate() {
    vs->activate();
    ps->activate();
  }
};

extern CVertexShader vs;
extern CPixelShader ps;
extern CVertexShader vs_obj;
extern CPixelShader ps_obj;

CRenderTechnique tech_solid = { &vs, &ps };
CRenderTechnique tech_objs = { &vs_obj, &ps_obj };

struct TEntity {
  std::string  name;
  TTransform   transform;
  CRenderMesh* mesh = nullptr;
  CRenderTechnique* tech = nullptr;
};

std::vector< TEntity > entities;
CCamera camera;

#include "render/cte_buffer.h"
#include "ctes.h"
CRenderCte<CCteCamera> cb_camera;


bool CModuleTestAxis::start()
{
  entities.clear();
  TEntity e;

  e.name = "john";
  e.transform.pos = VEC3(0, 0, 1);
  e.mesh = axis;
  e.tech = &tech_solid;
  entities.push_back(e);

  e.name = "peter";
  e.transform.pos = VEC3(2, 0, -1);
  e.mesh = loadMesh("data/meshes/Teapot001.mesh");
  e.tech = &tech_objs;
  entities.push_back(e);

  camera.lookAt(VEC3(2.0f, 2.0f, 3.0f), VEC3::Zero, VEC3::UnitY);
  camera.setPerspective(60.0f * 180.f / (float)M_PI, 0.1f, 1000.f);

  // -------------------------------------------
  if (!cb_camera.create(CB_CAMERAS))
    return false;
  return true;
}

bool CModuleTestAxis::stop()
{
  cb_camera.destroy();
  return true;
}

void CModuleTestAxis::update(float delta)
{

  if (ImGui::TreeNode("Entities")) {

    TEntity* e0 = nullptr;

    for (auto& e : entities) {
      ImGui::PushID(&e);
      if (ImGui::TreeNode(e.name.c_str())) {

        auto& t = e.transform;
        ImGui::DragFloat3("Pos", &t.pos.x, 0.025f, -50.f, 50.f);

        auto& front = t.getFront();
        ImGui::LabelText("Front", "%1.3f %1.3f %1.3f", front.x, front.y, front.z);

        // All entities except the first one, check if are in the front/left of the first entity
        if (e0) {
          bool is_e0_in_front_of_me = t.isInFront(e0->transform.pos);
          bool is_e0_in_left_of_me = t.isInLeft(e0->transform.pos);
          ImGui::LabelText("In Front", "%s", is_e0_in_front_of_me ? "YES" : "NO");
          ImGui::LabelText("In Left", "%s", is_e0_in_left_of_me ? "YES" : "NO" );
        }

        {
          float angle = rad2deg(t.yaw);
          if (ImGui::DragFloat("Yaw", &angle, 1.0f, -180.f, 180.f))
            e.transform.yaw = deg2rad(angle);
        }
        {
          float angle = rad2deg(t.pitch);
          if (ImGui::DragFloat("Pitch", &angle, 1.0f, -90.f, 90.f))
            e.transform.pitch = deg2rad(angle);
        }

        ImGui::TreePop();
      }

      if (!e0)
        e0 = &e;

      ImGui::PopID();
    }
    ImGui::TreePop();
  }

  if (ImGui::TreeNode("Camera")) {
    VEC3 new_pos = camera.getPosition();
    VEC3 new_target = camera.getTarget();
    bool changed = false;
    changed |= ImGui::DragFloat3("Pos", &new_pos.x, 0.025f, -50.f, 50.f);
    changed |= ImGui::DragFloat3("Target", &new_target.x, 0.025f, -50.f, 50.f);
    if( changed ) 
      camera.lookAt(new_pos, new_target);
    ImGui::TreePop();
  }

}


void CModuleTestAxis::render()
{
  tech_solid.activate();

  cb_camera.activate();
  cb_camera.view = camera.getView();
  cb_camera.proj = camera.getProjection();

  // Render the grid
  cb_camera.world = MAT44::Identity;
  cb_camera.updateGPU();
  grid->activateAndRender();

  for (auto& e : entities) {
    cb_camera.world = e.transform.getWorld();
    cb_camera.updateGPU();
    e.tech->activate();
    e.mesh->activateAndRender();
  }

}