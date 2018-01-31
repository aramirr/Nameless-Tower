#include "mcv_platform.h"
#include "entity_parser.h"
#include "entity.h"

bool parseScene(const std::string& filename, TEntityParseContext& ctx) {

  ctx.filename = filename;

  json j_scene = loadJson(filename);
  assert(j_scene.is_array());

  // For each item in the array...
  for (int i = 0; i < j_scene.size(); ++i ) {
    json& j_item = j_scene[i];

    assert(j_item.is_object());
    if (j_item.count("entity")) {
      json& j_entity = j_item["entity"];

      // Create a new fresh entity
      CHandle h_e;
      h_e.create< CEntity >();
      
      // Cast to entity object
      CEntity* e = h_e;

      // Do the parse
      e->load(j_entity, ctx);

      ctx.entities_loaded.push_back(h_e);
    }

  }

  return true;
}