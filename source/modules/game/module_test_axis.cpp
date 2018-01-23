#include "mcv_platform.h"
#include "module_test_axis.h"
#include "camera/camera.h"
#include "render/render_objects.h"
#include "render/mesh/mesh_loader.h"

// pos_loc * world * view * proj = pos_proj
// / wp => pos_homo

extern CVertexShader vs;
extern CPixelShader ps;
extern CVertexShader vs_obj;
extern CPixelShader ps_obj;

CRenderTechnique tech_solid = { &vs, &ps };
CRenderTechnique tech_objs = { &vs_obj, &ps_obj };

struct TEntity {
  std::string  name;
  CTransform   transform;
  CRenderMesh* mesh = nullptr;
  CRenderTechnique* tech = nullptr;
};

void from_json(const json& j, TEntity& p) {
	p.name = j.at("name").get<std::string>();
	if (j.count("transform"))
		p.transform.load(j["transform"]);
	if (j.count("mesh")) {
		std::string name = j["mesh"];
		p.mesh = loadMesh(name.c_str());
		p.tech = &tech_objs;
	}
	else {
		p.mesh = axis;
		p.tech = &tech_solid;
	}
}

std::vector< TEntity > entities;
CCamera camera;

bool CModuleTestAxis::start()
{
  entities.clear();

  // Load json with entities array
  json j_entities = loadJson("data/entities.json");

  // Convert the json to an array of entities
  entities = j_entities.get< std::vector<TEntity> >();

  camera.lookAt(VEC3(12.0f, 8.0f, 8.0f), VEC3::Zero, VEC3::UnitY);
  camera.setPerspective(60.0f * 180.f / (float)M_PI, 0.1f, 1000.f);

  // -------------------------------------------
  if (!cb_camera.create(CB_CAMERA))
    return false;
  // -------------------------------------------
  if (!cb_object.create(CB_OBJECT))
    return false;

  cb_object.activate();
  cb_camera.activate();

  return true;
}

bool CModuleTestAxis::stop()
{
  cb_camera.destroy();
  cb_object.destroy();
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
				t.debugInMenu();
        // All entities except the first one, check if are in the front/left of the first entity
        if (e0) {
          bool is_e0_in_front_of_me = t.isInFront(e0->transform.getPosition());
          bool is_e0_in_left_of_me = t.isInLeft(e0->transform.getPosition());
          ImGui::LabelText("In Front", "%s", is_e0_in_front_of_me ? "YES" : "NO");
          ImGui::LabelText("In Left", "%s", is_e0_in_left_of_me ? "YES" : "NO" );
          float delta_yaw_to_e0 = t.getDeltaYawToAimTo(e0->transform.getPosition());
          ImGui::LabelText("Delta To e0", "%f", rad2deg( delta_yaw_to_e0 ));

          if (ImGui::Button("Aim To..")) {
            float old_yaw, old_pitch;
            t.getYawPitchRoll(&old_yaw, &old_pitch);
            float new_yaw = old_yaw + delta_yaw_to_e0 * 0.15f;
            t.setYawPitchRoll(new_yaw, old_pitch);
          }
          static float fov = 90.f;
          ImGui::SameLine();
          bool in_in_fov = t.isInFov(e0->transform.getPosition(), deg2rad(fov));
          ImGui::Text("Inside:%d", in_in_fov);
          ImGui::DragFloat("Fov", &fov, 1.0f, 1.f, 120.f);
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

  activateCamera(camera);

  // Render the grid
  cb_object.obj_world = MAT44::Identity;
  cb_object.obj_color = VEC4(1,1,1,1);
  cb_object.updateGPU();
  grid->activateAndRender();
  axis->activateAndRender();

  for (auto& e : entities) {
    cb_object.obj_world = e.transform.asMatrix();
    //cb_object.obj_color = e.color
    cb_object.updateGPU();
    e.tech->activate();
    e.mesh->activateAndRender();
  }

}