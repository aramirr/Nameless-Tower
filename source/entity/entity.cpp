#include "mcv_platform.h"
#include "entity.h"
#include "render/mesh/mesh_loader.h"
#include "render/render_objects.h"
#include "modules/module_ia.h"

extern CVertexShader vs;
extern CPixelShader ps;
extern CVertexShader vs_obj;
extern CPixelShader ps_obj;

CRenderTechnique tech_solid = { &vs, &ps };
CRenderTechnique tech_objs = { &vs_obj, &ps_obj };
  
void TEntity::load(const json& j) {

  name = j.at("name").get<std::string>();

  if (j.count("transform"))
    transform.load(j["transform"]);

  // Load some render mesh
  if (j.count("mesh")) {
    std::string name = j["mesh"];
    mesh = loadMesh(name.c_str());
    tech = &tech_objs;
  }
  else {
    mesh = axis;
    tech = &tech_solid;
  }

  // Load an AI controller
  if (j.count("ia")) {
    // Get a new instance of the controller based on the json params and bind it to entity p
    ai_controller = CEngine::get().getIA().getNewAIControler( j["ia"], this );
  }

}
