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
  mesh = new TInstancedDecals();
  auto instanced_mesh = Resources.get("data/meshes/GeoSphere001.mesh")->as<CRenderMesh>();
  mesh->setInstancedMesh(instanced_mesh);
  mesh->cpu_instances.reserve(4);
  mesh->uploadInstancesToGPU();
  mesh->setNameAndClass("sphere_decals.mesh", getResourceClassOf<CRenderMesh>());
  Resources.registerResource(mesh);

  return true;
}

void CModuleTestInstancing::update(float delta)
{
  if( ImGui::TreeNode( "Decals") ) {
    ImGui::Text("Num Instances: %ld", mesh->cpu_instances.size());
    static float sz = 10.f;
    ImGui::DragFloat("Size", &sz, 0.01f, -10.f, 10.f);
    if (ImGui::Button("Add")) {
      TRenderDecal3D new_decal;
      new_decal.world = MAT44::CreateTranslation(VEC3(randomFloat(-sz, sz), 0, randomFloat(-sz, sz)));
      mesh->cpu_instances.push_back(new_decal);
      mesh->setSubGroupSize(0, (uint32_t)mesh->cpu_instances.size());
    }
    ImGui::TreePop();
  }


  static float t = 0;
  t += delta;
  for (auto& p : mesh->cpu_instances) 
    p.world = p.world * MAT44::CreateTranslation(VEC3(0, 0.1f * sin(t), 0));
  mesh->uploadInstancesToGPU();

}
