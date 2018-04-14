#include "mcv_platform.h"
#include "module_test_instancing.h"
#include "geometry/transform.h"

float unitRandom() {
  return (float)rand() / (float)RAND_MAX;
}

float randomFloat( float vmin, float vmax) {
  return vmin + (vmax - vmin) * unitRandom();
}


bool CModuleTestInstancing::start()
{
  auto instanced_mesh = Resources.get("data/meshes/GeoSphere001.mesh")->as<CRenderMesh>();
  
  mesh = new TInstancedMeshes();
  mesh->setInstancedMesh(instanced_mesh);
  mesh->cpu_instances.reserve(4);
  mesh->reserveGPUInstances(4);
  mesh->setNameAndClass("sphere_instanced.mesh", getResourceClassOf<CRenderMesh>());
  Resources.registerResource(mesh);

  return true;
}

void CModuleTestInstancing::update(float delta)
{
  if( ImGui::TreeNode( "Instances") ) {
    ImGui::Text("Num Instances: %ld / %ld. GPU:%d", mesh->cpu_instances.size(), mesh->cpu_instances.capacity(), mesh->getVertexsCount() );
    static float sz = 50.f;
    static int num = 3;
    ImGui::DragFloat("Size", &sz, 0.01f, -50.f, 50.f);
    ImGui::DragInt("Num", &num, 0.1f, 1, 10);
    if (ImGui::Button("Add")) {
      for (int i = 0; i < num; ++i) {
        TInstance new_instance;
        new_instance.world = MAT44::CreateTranslation(VEC3(randomFloat(-sz, sz), 0, randomFloat(-sz, sz)));
        mesh->cpu_instances.push_back(new_instance);
      }
    }
    ImGui::SameLine();
    if (ImGui::Button("Del") && !mesh->cpu_instances.empty()) {
      mesh->cpu_instances.pop_back();
    }
    ImGui::TreePop();
  }

  // Move the instances in the cpu
  static float t = 0;
  t += delta;
  for (auto& p : mesh->cpu_instances) 
    p.world = p.world * MAT44::CreateTranslation(VEC3(0, 0.1f * sin(t), 0));
  mesh->uploadInstancesToGPU();

}
