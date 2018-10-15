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
	    auto rmesh = Resources.get("data/meshes/particles_15.instanced_mesh")->as<CRenderMesh>();
	    fire_particles_instances_mesh = (CRenderMeshInstanced*)rmesh;
    }
    {
        auto rmesh = Resources.get("data/meshes/particles_15_violeta.instanced_mesh")->as<CRenderMesh>();
        fire_particles_violeta_instances_mesh = (CRenderMeshInstanced*)rmesh;
    }
    {
        auto rmesh = Resources.get("data/meshes/particles_15_azul.instanced_mesh")->as<CRenderMesh>();
        fire_particles_azul_instances_mesh = (CRenderMeshInstanced*)rmesh;
    }
    {
        auto rmesh = Resources.get("data/meshes/particles_thin_15.instanced_mesh")->as<CRenderMesh>();
        thin_fire_particles_instances_mesh = (CRenderMeshInstanced*)rmesh;
    }
    {
        auto rmesh = Resources.get("data/meshes/particles_thin_15_violeta.instanced_mesh")->as<CRenderMesh>();
        thin_fire_violeta_particles_instances_mesh = (CRenderMeshInstanced*)rmesh;
    }
    {
        auto rmesh = Resources.get("data/meshes/particles_thin_15_azul.instanced_mesh")->as<CRenderMesh>();
        thin_fire_azul_particles_instances_mesh = (CRenderMeshInstanced*)rmesh;
    }
    {
        auto rmesh = Resources.get("data/meshes/particles_4_azul_apagando.instanced_mesh")->as<CRenderMesh>();
        thin_fire_azul_apagando_particles_instances_mesh = (CRenderMeshInstanced*)rmesh;
    }
    {
        auto rmesh = Resources.get("data/meshes/smoke_4_prendiendo.instanced_mesh")->as<CRenderMesh>();
        smoke_4_prendiendo_particles_instances_mesh = (CRenderMeshInstanced*)rmesh;
    }
    {
        auto rmesh = Resources.get("data/meshes/particles_thin_14.instanced_mesh")->as<CRenderMesh>();
        thin_smoke_particles_instances_mesh = (CRenderMeshInstanced*)rmesh;
    }
	{
		//auto rmesh = Resources.get("data/meshes/particles_14.instanced_mesh")->as<CRenderMesh>();
		//smoke_particles_instances_mesh = (CRenderMeshInstanced*)rmesh;
	}
  {
    auto rmesh = Resources.get("data/meshes/grass.instanced_mesh")->as<CRenderMesh>();
    grass_instances_mesh = (CRenderMeshInstanced*)rmesh;
  }
  {
	  auto rmesh = Resources.get("data/meshes/windstrike.instanced_mesh")->as<CRenderMesh>();
	  windstrike_instances_mesh = (CRenderMeshInstanced*)rmesh;
  }
  return true;
}

void CModuleBillboards::apagarFuego(int id, float scale, bool thin) {
    if (thin) {
        for (int i = 0; i < thin_fire_particles_instances.size(); ++i) {
            if (thin_fire_particles_ids[i] == id) {
                thin_fire_particles_instances[i].scale_x = 0.f;
                thin_fire_particles_instances[i].scale_y = 0.f;
            }
        }
    }
    else {
        for (int i = 0; i < fire_particles_instances.size(); ++i) {
            if (fire_particles_ids[i] == id) {
                fire_particles_instances[i].scale_x = 0.f;
                fire_particles_instances[i].scale_y = 0.f;
            }
        }
    }	
}

void CModuleBillboards::apagarFuegoAzul(int id, float scale, VEC3 position, float y_offset) {
    for (int i = 0; i < thin_fire_azul_particles_instances.size(); ++i) {
        if (thin_fire_azul_apagando_particles_ids[i] == id) {
            TRenderParticle humo_prendiendo_instance;
            humo_prendiendo_instance.scale_x = scale;
            humo_prendiendo_instance.scale_y = humo_prendiendo_instance.scale_x;
            humo_prendiendo_instance.pos = position + VEC3(0.1, y_offset, 0);
            humo_prendiendo_instance.nframe = 0;
            humo_prendiendo_instance.angle = deg2rad(randomFloat(0, 360));
            humo_prendiendo_instance.color.x = unitRandom();
            humo_prendiendo_instance.color.y = unitRandom();
            humo_prendiendo_instance.color.z = 1 - humo_prendiendo_instance.color.x - humo_prendiendo_instance.color.y;
            humo_prendiendo_instance.color.w = 1;

            smoke_4_prendiendo_particles_instances.push_back(humo_prendiendo_instance);
            smoke_prendiendo_particles_ids.push_back(id);
            auto it = thin_fire_azul_apagando_particles_instances.begin();
            while (it != _particles.end())
            {
            thin_fire_azul_apagando_particles_instances.erase(thin_fire_azul_apagando_particles_instances[thin_fire_azul_apagando_particles_ids.begin() + i]);
        }
    }   
}

void CModuleBillboards::apagandoFuegoAzul(int id, float scale, VEC3 position, float y_offset) {
    for (int i = 0; i < thin_fire_azul_particles_instances.size(); ++i) {
        if (thin_fire_azul_particles_ids[i] == id) {
            TRenderParticle fire_apagando_instance;
            fire_apagando_instance.scale_x = scale;
            fire_apagando_instance.scale_y = fire_apagando_instance.scale_x;
            fire_apagando_instance.pos = position;
            fire_apagando_instance.nframe = 0;
            fire_apagando_instance.angle = deg2rad(randomFloat(0, 360));
            fire_apagando_instance.color.x = unitRandom();
            fire_apagando_instance.color.y = unitRandom();
            fire_apagando_instance.color.z = 1 - fire_apagando_instance.color.x - fire_apagando_instance.color.y;
            fire_apagando_instance.color.w = 1;

            thin_fire_azul_apagando_particles_instances.push_back(fire_apagando_instance);
            thin_fire_azul_apagando_particles_ids.push_back(id);
            thin_fire_azul_particles_instances[i].scale_x = 0.f;
            thin_fire_azul_particles_instances[i].scale_y = 0.f;
        }
    }
}

void CModuleBillboards::prendiendoHumo(int id, float scale, VEC3 position, float y_offset) {
    for (int i = 0; i < thin_fire_azul_particles_instances.size(); ++i) {
        if (smoke_prendiendo_particles_ids[i] == id) {
            TRenderParticle smoke_instance;
            smoke_instance.scale_x = scale;
            smoke_instance.scale_y = smoke_instance.scale_x;
            smoke_instance.pos = position + VEC3(0, y_offset, 0);
            smoke_instance.nframe = 0;
            smoke_instance.angle = deg2rad(randomFloat(0, 360));
            smoke_instance.color.x = unitRandom();
            smoke_instance.color.y = unitRandom();
            smoke_instance.color.z = 1 - smoke_instance.color.x - smoke_instance.color.y;
            smoke_instance.color.w = 1;

            thin_smoke_particles_instances.push_back(smoke_instance);
            thin_smoke_particles_ids.push_back(id);
            smoke_prendiendo_particles_ids.erase(smoke_prendiendo_particles_ids.begin() + i);
            smoke_4_prendiendo_particles_instances.erase(smoke_4_prendiendo_particles_instances[smoke_prendiendo_particles_ids.begin() + i]);
        }
    }
}

void CModuleBillboards::encenderFuego(int id, float scale, bool thin) {
    if (thin) {
        for (int i = 0; i < thin_fire_particles_instances.size(); ++i) {
            if (thin_fire_particles_ids[i] == id) {
                thin_fire_particles_instances[i].scale_x = scale;
                thin_fire_particles_instances[i].scale_y = scale;
            }
        }
    }
    else {
        for (int i = 0; i < fire_particles_instances.size(); ++i) {
            if (fire_particles_ids[i] == id) {
                fire_particles_instances[i].scale_x = scale;
                fire_particles_instances[i].scale_y = scale;
            }
        }
    }
	
}

void CModuleBillboards::encenderFuegoVioleta(int id, float scale, bool thin) {
    if (thin) {
        for (int i = 0; i < thin_fire_violeta_particles_instances.size(); ++i) {
            if (thin_fire_violeta_particles_ids[i] == id) {
                thin_fire_violeta_particles_instances[i].scale_x = scale;
                thin_fire_violeta_particles_instances[i].scale_y = scale;
            }
        }
    }
    else {
        for (int i = 0; i < fire_violeta_particles_instances.size(); ++i) {
            if (fire_violeta_particles_ids[i] == id) {
                fire_violeta_particles_instances[i].scale_x = scale;
                fire_violeta_particles_instances[i].scale_y = scale;
            }
        }
    }

}

void CModuleBillboards::encenderFuegoAzul(int id, float scale) {
    for (int i = 0; i < thin_fire_azul_particles_instances.size(); ++i) {
        if (thin_fire_azul_particles_ids[i] == id) {
            thin_fire_azul_particles_instances[i].scale_x = scale;
            thin_fire_azul_particles_instances[i].scale_y = scale;
        }
    }
    for (int i = 0; i < thin_smoke_particles_instances.size(); ++i) {
        if (thin_smoke_particles_ids[i] == id) {
            thin_smoke_particles_instances[i].scale_x = 0.f;
            thin_smoke_particles_instances[i].scale_y = 0.f;
        }
    }
}

int CModuleBillboards::addFuegoTest(VEC3 position, float scale, bool thin) {
	int fire_new_id = fuego_max_id;
	++fuego_max_id;
	TRenderParticle new_instance;
	//new_instance.id = new_id;
	new_instance.scale_x = scale;
  new_instance.scale_y = new_instance.scale_x;
  new_instance.pos = position;
  new_instance.nframe = randomFloat(0.f, 16.f);
  new_instance.angle = deg2rad(randomFloat(0, 360));
  new_instance.color.x = unitRandom();
  new_instance.color.y = unitRandom();
  new_instance.color.z = 1 - new_instance.color.x - new_instance.color.y;
  new_instance.color.w = 1;
	TRenderParticle smoke_instance;
	//new_instance.id = new_id;
	smoke_instance.scale_x = 0.f;
	smoke_instance.scale_y = smoke_instance.scale_x;
	smoke_instance.pos = position;
	smoke_instance.nframe = randomFloat(0.f, 16.f);
	smoke_instance.angle = deg2rad(randomFloat(0, 360));
	smoke_instance.color.x = unitRandom();
	smoke_instance.color.y = unitRandom();
	smoke_instance.color.z = 1 - smoke_instance.color.x - smoke_instance.color.y;
	smoke_instance.color.w = 1;
    if (thin) {
        thin_fire_particles_instances.push_back(new_instance);
        thin_fire_particles_ids.push_back(fire_new_id);
    }
    else {
        fire_particles_instances.push_back(new_instance);
        fire_particles_ids.push_back(fire_new_id);
    }	

	return fire_new_id;
}

int CModuleBillboards::addFuegoVioleta(VEC3 position, float scale, bool thin) {
    int fire_new_id = fuego_max_id;
    ++fuego_max_id;
    TRenderParticle new_instance;
    //new_instance.id = new_id;
    new_instance.scale_x = scale;
    new_instance.scale_y = new_instance.scale_x;
    new_instance.pos = position;
    new_instance.nframe = randomFloat(0.f, 16.f);
    new_instance.angle = deg2rad(randomFloat(0, 360));
    new_instance.color.x = unitRandom();
    new_instance.color.y = unitRandom();
    new_instance.color.z = 1 - new_instance.color.x - new_instance.color.y;
    new_instance.color.w = 1;
    TRenderParticle smoke_instance;
    //new_instance.id = new_id;
    smoke_instance.scale_x = 0.f;
    smoke_instance.scale_y = smoke_instance.scale_x;
    smoke_instance.pos = position;
    smoke_instance.nframe = randomFloat(0.f, 16.f);
    smoke_instance.angle = deg2rad(randomFloat(0, 360));
    smoke_instance.color.x = unitRandom();
    smoke_instance.color.y = unitRandom();
    smoke_instance.color.z = 1 - smoke_instance.color.x - smoke_instance.color.y;
    smoke_instance.color.w = 1;
    if (thin) {
        thin_fire_violeta_particles_instances.push_back(new_instance);
        thin_fire_violeta_particles_ids.push_back(fire_new_id);
    }
    else {
        fire_violeta_particles_instances.push_back(new_instance);
        fire_violeta_particles_ids.push_back(fire_new_id);
    }

    return fire_new_id;
}

int CModuleBillboards::addFuegoAzul(VEC3 position, float scale, float smoke_y_offset) {
    int fire_new_id = fuego_max_id;
    ++fuego_max_id;
    TRenderParticle new_instance;
    //new_instance.id = new_id;
    new_instance.scale_x = scale;
    new_instance.scale_y = new_instance.scale_x;
    new_instance.pos = position;
    new_instance.nframe = 0;
    new_instance.angle = deg2rad(randomFloat(0, 360));
    new_instance.color.x = unitRandom();
    new_instance.color.y = unitRandom();
    new_instance.color.z = 1 - new_instance.color.x - new_instance.color.y;
    new_instance.color.w = 1;       

    thin_fire_azul_particles_instances.push_back(new_instance);
    thin_fire_azul_particles_ids.push_back(fire_new_id);

    return fire_new_id;
}

int CModuleBillboards::addWindstrike(VEC3 position) {
	int new_id = windstrike_max_id;
	++windstrike_max_id;
	TWindstrikeParticle new_instance;
	new_instance.scale_x = 1.0f;
	new_instance.scale_y = new_instance.scale_x;
	new_instance.pos = position;
	new_instance.nframe = randomFloat(0.f, 16.f);
	new_instance.angle = deg2rad(randomFloat(0, 360));
	new_instance.color.x = unitRandom();
	new_instance.color.y = unitRandom();
	new_instance.color.z = 1 - new_instance.color.x - new_instance.color.y;
	new_instance.color.w = 1;


	windstrike_instances.push_back(new_instance);
	windstrike_ids.push_back(new_id);

	return new_id;
}

void CModuleBillboards::moveWindstrike(VEC3 position, int id) {
	for (int i = 0; i < windstrike_instances.size(); ++i) {
		if (windstrike_ids[i] == id) {
			windstrike_instances[i].pos = position;
		}
	}
}

void CModuleBillboards::deleteWindstrike(int id) {
	for (int i = 0; i < windstrike_instances.size(); ++i) {
		if (windstrike_ids[i] == id) {
			windstrike_instances.erase(windstrike_instances.begin() + i);
			windstrike_ids.erase(windstrike_ids.begin() + i);
		}
	}
}

void CModuleBillboards::addGrass(VEC3 position, float width, float length, int total) {
	for (int i = 0; i < total; ++i) {
		TGrassParticle new_instance;
		new_instance.pos = VEC3(position.x + randomFloat(-width, width), position.y, position.z + randomFloat(-length, length));
		grass_instances.push_back(new_instance);
	}
	grass_instances_mesh->setInstancesData(grass_instances.data(), grass_instances.size(), sizeof(TGrassParticle));
}

void CModuleBillboards::addGrassByAngle(VEC3 pos1, VEC3 pos2, int total, float radius1, float radius2) {
  float alpha = asin(pos1.z / EngineTower.getTowerRadius());
  alpha = EngineTower.checkAngle(alpha, pos1);
  float beta = asin(pos2.z / EngineTower.getTowerRadius());
  beta = EngineTower.checkAngle(beta, pos2);

  for (int i = 0; i < total; ++i) {
    float charlie;
    if (alpha < beta)
      charlie = randomFloat(alpha, beta);
    else {
      //charlie = randomFloat(beta, alpha);

      float proportion_beta = (beta / (deg2rad(360) - alpha + beta))*100;
      float aux = randomFloat(0, 100);
      if (aux > proportion_beta )
        charlie = randomFloat(alpha, deg2rad(360));
      else 
        charlie = randomFloat(deg2rad(0), beta);
    }

    float aux_radius = randomFloat(radius1, radius2);
    VEC3 new_position = VEC3(aux_radius*cos(charlie), pos1.y, aux_radius*sin(charlie));
    TGrassParticle new_instance;
    new_instance.pos = new_position;
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
  //for (auto& p : particles_instances) {
      //p.angle += delta;
      
  //}

  // Move the instances in the cpu
  static float t = 0;
  t += delta;
  for (auto& p : instances) 
    p.world = p.world * MAT44::CreateTranslation(VEC3(0, 0.1f * sin(t), 0));
    instances_mesh->setInstancesData(instances.data(), instances.size(), sizeof(TInstance));

    blood_instances_mesh->setInstancesData(blood_instances.data(), blood_instances.size(), sizeof(TInstanceBlood));  
    particles_instances_mesh->setInstancesData(particles_instances.data(), particles_instances.size(), sizeof(TRenderParticle));
    fire_particles_instances_mesh->setInstancesData(fire_particles_instances.data(), fire_particles_instances.size(), sizeof(TRenderParticle));
    fire_particles_violeta_instances_mesh->setInstancesData(fire_violeta_particles_instances.data(), fire_violeta_particles_instances.size(), sizeof(TRenderParticle));
    fire_particles_azul_instances_mesh->setInstancesData(fire_azul_particles_instances.data(), fire_azul_particles_instances.size(), sizeof(TRenderParticle));
    thin_fire_particles_instances_mesh->setInstancesData(thin_fire_particles_instances.data(), thin_fire_particles_instances.size(), sizeof(TRenderParticle));
    thin_fire_violeta_particles_instances_mesh->setInstancesData(thin_fire_violeta_particles_instances.data(), thin_fire_violeta_particles_instances.size(), sizeof(TRenderParticle));
    thin_fire_azul_particles_instances_mesh->setInstancesData(thin_fire_azul_particles_instances.data(), thin_fire_azul_particles_instances.size(), sizeof(TRenderParticle));
    thin_fire_azul_apagando_particles_instances_mesh->setInstancesData(thin_fire_azul_apagando_particles_instances.data(), thin_fire_azul_apagando_particles_instances.size(), sizeof(TRenderParticle));
    smoke_4_prendiendo_particles_instances_mesh->setInstancesData(smoke_4_prendiendo_particles_instances.data(), smoke_4_prendiendo_particles_instances.size(), sizeof(TRenderParticle));
    //smoke_particles_instances_mesh->setInstancesData(smoke_particles_instances.data(), smoke_particles_instances.size(), sizeof(TRenderParticle));
    thin_smoke_particles_instances_mesh->setInstancesData(thin_smoke_particles_instances.data(), thin_smoke_particles_instances.size(), sizeof(TRenderParticle));
    windstrike_instances_mesh->setInstancesData(windstrike_instances.data(), windstrike_instances.size(), sizeof(TWindstrikeParticle));

}

void CModuleBillboards::clearFire() {
    fire_particles_instances.clear();
    thin_fire_particles_instances.clear();
}