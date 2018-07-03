#include "mcv_platform.h"
#include "module_billboards.h"
#include "geometry/transform.h"
#include "entity/entity_parser.h"
#include "components/juan/comp_transform.h"

float unitRandom() {
  return (float)rand() / (float)RAND_MAX;
}

float randomFloat( float vmin, float vmax) {
  return vmin + (vmax - vmin) * unitRandom();
}

bool CModuleBillboards::start()
{
  {
    auto rmesh = Resources.get("data/meshes/GeoSphere001.instanced_mesh")->as<CRenderMesh>();
    // Remove cast and upcast to CRenderMeshInstanced
    instances_mesh = (CRenderMeshInstanced*)rmesh;
  }
  {
    auto rmesh = Resources.get("data/meshes/blood.instanced_mesh")->as<CRenderMesh>();
    blood_instances_mesh = (CRenderMeshInstanced*)rmesh;
  }
  {
    auto rmesh = Resources.get("data/meshes/particles.instanced_mesh")->as<CRenderMesh>();
    particles_instances_mesh = (CRenderMeshInstanced*)rmesh;
  }
  {
    auto rmesh = Resources.get("data/meshes/grass.instanced_mesh")->as<CRenderMesh>();
    grass_instances_mesh = (CRenderMeshInstanced*)rmesh;
  }
  return true;
}

void CModuleBillboards::addFuegoTest(VEC3 position) {
    TRenderParticle new_instance;
    new_instance.scale_x = randomFloat(0.3f, 0.5f);
    new_instance.scale_y = new_instance.scale_x;
    new_instance.pos = position;
    new_instance.nframe = randomFloat(0.f, 16.f);
    new_instance.angle = deg2rad(randomFloat(0, 360));
    new_instance.color.x = unitRandom();
    new_instance.color.y = unitRandom();
    new_instance.color.z = 1 - new_instance.color.x - new_instance.color.y;
    new_instance.color.w = 1;
    particles_instances.push_back(new_instance);
}

void CModuleBillboards::addGrass(VEC3 position, float width, float length, int total) {
	for (int i = 0; i < total; ++i) {
		TGrassParticle new_instance;
		new_instance.pos = VEC3(position.x + randomFloat(-width, width), position.y, position.z + randomFloat(-length, length));
		grass_instances.push_back(new_instance);
	}
	grass_instances_mesh->setInstancesData(grass_instances.data(), grass_instances.size(), sizeof(TGrassParticle));
}


void CModuleBillboards::update(float delta)
{

  if (ImGui::TreeNode("Instancing")) {

    // -- Creation params ----------------------
    static float sz = 50.f;
    static int num = 3;
    ImGui::DragFloat("Size", &sz, 0.01f, -50.f, 50.f);
    ImGui::DragInt("Num", &num, 0.1f, 1, 10);

    // ----------------------------------------------
    if (ImGui::TreeNode("Particles")) {
      ImGui::Text("Num Instances: %ld / %ld. GPU:%d", particles_instances.size(), particles_instances.capacity(), particles_instances_mesh->getVertexsCount());

      if (ImGui::Button("Add")) {
        for (int i = 0; i < num; ++i) {
          TRenderParticle new_instance;
          new_instance.scale_x = randomFloat(1.f, 5.f);
          new_instance.scale_y = new_instance.scale_x;
          new_instance.pos = VEC3(randomFloat(-sz, sz), new_instance.scale_x, randomFloat(-sz, sz));
          new_instance.nframe = randomFloat(0.f, 16.f);
          new_instance.angle = deg2rad(randomFloat(0, 360));
          new_instance.color.x = unitRandom();
          new_instance.color.y = unitRandom();
          new_instance.color.z = 1 - new_instance.color.x - new_instance.color.y;
          new_instance.color.w = 1;
          particles_instances.push_back(new_instance);
        }
      }
      ImGui::SameLine();
      if (ImGui::Button("Del") && !particles_instances.empty())
        particles_instances.pop_back();
      ImGui::TreePop();
    }

    // ----------------------------------------------
    if (ImGui::TreeNode("Spheres")) {
      ImGui::Text("Num Instances: %ld / %ld. GPU:%d", instances.size(), instances.capacity(), instances_mesh->getVertexsCount());

      if (ImGui::Button("Add")) {
        for (int i = 0; i < num; ++i) {
          TInstance new_instance;
          new_instance.world = MAT44::CreateTranslation(VEC3(randomFloat(-sz, sz), 0, randomFloat(-sz, sz)));
          instances.push_back(new_instance);
        }
      }
      ImGui::SameLine();
      if (ImGui::Button("Del") && !instances.empty())
        instances.pop_back();
      ImGui::TreePop();
    }

    // ----------------------------------------------
    if (ImGui::TreeNode("Grass")) {
      bool changed = false;
      ImGui::Text("Num Instances: %ld / %ld. GPU:%d", grass_instances.size(), grass_instances.capacity(), grass_instances_mesh->getVertexsCount());
      int num_changed = num * 100;
      if (ImGui::Button("Add 100")) {
        for (int i = 0; i < num_changed; ++i) {
          TGrassParticle new_instance;
          new_instance.pos = VEC3(randomFloat(-sz, sz), 0, randomFloat(-sz, sz));
          grass_instances.push_back(new_instance);
        }
        changed = true;
      }
      ImGui::SameLine();
      if (ImGui::Button("Del") && !instances.empty()) {
        if (num_changed < instances.size())
          num_changed = instances.size();
        instances.resize(instances.size() - num_changed);
        changed = true;
      }
      ImGui::TreePop();

      // Update GPU with the new CPU
      if (changed)
        grass_instances_mesh->setInstancesData(grass_instances.data(), grass_instances.size(), sizeof(TGrassParticle));
    }

    // ----------------------------------------------
    if (ImGui::TreeNode("Blood")) {
      ImGui::Text("Num Instances: %ld / %ld. GPU:%d", blood_instances.size(), blood_instances.capacity(), blood_instances_mesh->getVertexsCount());
      if (ImGui::Button("Add")) {
        for (int i = 0; i < num; ++i) {
          TInstanceBlood new_instance;
          new_instance.world =
            MAT44::CreateRotationY(randomFloat((float)-M_PI, (float)M_PI))
            *
            MAT44::CreateScale(randomFloat(2.f, 10.f))
            *
            MAT44::CreateTranslation(VEC3(randomFloat(-sz, sz), 0, randomFloat(-sz, sz)));
          new_instance.color.x = unitRandom();
          new_instance.color.y = unitRandom();
          new_instance.color.z = 1 - new_instance.color.x - new_instance.color.y;
          new_instance.color.w = 1;
          blood_instances.push_back(new_instance);
        }
      }
      ImGui::SameLine();
      if (ImGui::Button("Del") && !blood_instances.empty())
        blood_instances.pop_back();
      if (ImGui::TreeNode("Instances")) {
        for (auto& p : blood_instances) {
          ImGui::PushID(&p);
          VEC3 scale, trans;
          QUAT rot;
          p.world.Decompose(scale, rot, trans);
          CTransform tmx;
          tmx.setRotation(rot);
          tmx.setPosition(trans);
          tmx.setScale(scale.x);
          if (tmx.debugInMenu())
            p.world = tmx.asMatrix();
          ImGui::ColorEdit4("Color", &p.color.x);
          ImGui::PopID();
        }
        ImGui::TreePop();
      }
      ImGui::TreePop();
    }

    // ----------------------------------------------
    if (ImGui::SmallButton("Make Teapots")) {
      std::string prefab = "data/prefabs/teapot_hi.prefab";
      for (int i = -num; i < num; ++i) {
        for (int j = -num; j < num; ++j) {
          VEC3 pos((float)i, 0, (float)j);
          TEntityParseContext ctx;
          parseScene(prefab, ctx);
          CEntity* e = ctx.entities_loaded[0];
          TCompTransform* t = e->get<TCompTransform>();
          t->setPosition(pos * sz);
        }
      }
    }

    ImGui::TreePop();
  }

  // Rotate the particles
  for (auto& p : particles_instances) {
      //p.angle += delta;
      
  }

  // Move the instances in the cpu
  static float t = 0;
  t += delta;
  for (auto& p : instances) 
    p.world = p.world * MAT44::CreateTranslation(VEC3(0, 0.1f * sin(t), 0));
  instances_mesh->setInstancesData(instances.data(), instances.size(), sizeof(TInstance));

  blood_instances_mesh->setInstancesData(blood_instances.data(), blood_instances.size(), sizeof(TInstanceBlood));
  
  particles_instances_mesh->setInstancesData(particles_instances.data(), particles_instances.size(), sizeof(TRenderParticle));
}
