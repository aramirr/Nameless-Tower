#include "mcv_platform.h"
#include "entity_parser.h"
#include "entity.h"
#include "common_msgs.h"
#include "resources/json_resource.h"
#include "components/comp_group.h"

// Find in the current list of entities created, the first entity matching
// the given name
CHandle TEntityParseContext::findEntityByName(const std::string& name) const {

  // Search linearly in the list of entity currently loaded
  for (auto h : entities_loaded) {
    CEntity* e = h;
    if (e->getName() == name )
      return h;
  }

  // Delegate it to my parent
  if (parent)
    return parent->findEntityByName(name);

  return getEntityByName(name);
}

// 
bool parseScene(const std::string& filename, TEntityParseContext& ctx) {

  ctx.filename = filename;

  const json& j_scene = Resources.get(filename)->as<CJsonResource>()->getJson();
  assert(j_scene.is_array());

  // For each item in the array...
  for (int i = 0; i < j_scene.size(); ++i ) {
    auto& j_item = j_scene[i];

    assert(j_item.is_object());
    if (j_item.count("entity")) {
      auto& j_entity = j_item["entity"];

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

  // Create a comp_group automatically if there is more than one entity
  if (ctx.entities_loaded.size() > 1 ) {
    // The first entity becomes the head of the group. He is NOT in the group
    CHandle h_root_of_group = ctx.entities_loaded[0];
    CEntity* e_root_of_group = h_root_of_group;
    assert(e_root_of_group);
    // Create a new instance of the TCompGroup
    CHandle h_group = getObjectManager<TCompGroup>()->createHandle();
    // Add it to the entity
    e_root_of_group->set(h_group.getType(), h_group);
    // Now add the rest of entities created to the group, starting at 1 because 0 is the head
    TCompGroup* c_group = h_group;
    for (size_t i = 1; i < ctx.entities_loaded.size(); ++i)
      c_group->add(ctx.entities_loaded[i]);
  }

  // Notify each entity created that we have finished
  // processing this file
  TMsgEntitiesGroupCreated msg = { ctx };
  for (auto h : ctx.entities_loaded)
    h.sendMsg(msg);

  return true;
}