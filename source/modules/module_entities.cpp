#include "mcv_platform.h"
#include "module_entities.h"
#include "render/render_objects.h"
#include "render/texture/texture.h"
#include "entity/entity.h"
#include "components/comp_render.h"
#include "components/comp_transform.h"
#include "components/comp_name.h"



bool CModuleEntities::start()
{
  json j = loadJson("data/components.json");
  json j_sizes = j["sizes"];
  
  // Initialize the ObjManager preregistered in their constructors
  // with the amount of components defined in the data/components.json
  std::map< std::string, int > comp_sizes = j_sizes;
  int default_size = comp_sizes["default"];
  for (size_t i = 0; i < CHandleManager::npredefined_managers; ++i) {
    auto om = CHandleManager::predefined_managers[i];
    auto it = comp_sizes.find(om->getName());
    int sz = (it == comp_sizes.end()) ? default_size : it->second;
    dbg("Initializing obj manager %s with %d\n", om->getName(), sz);
    om->init(sz, false);
  }

  // For each entry in j["update"] add entry to om_to_update
  std::vector< std::string > names = j["update"];
  for (auto& n : names) {
    auto om = CHandleManager::getByName(n.c_str());
    assert(om);
    om_to_update.push_back(om);
  }

  return true;
}

void CModuleEntities::update(float delta)
{
  for (auto om : om_to_update)
    om->updateAll(delta);
}

void CModuleEntities::render()
{
  Resources.debugInMenu();

  auto om = getObjectManager<CEntity>();
  om->debugInMenuAll();

  // Do the basic render
  auto om_render = getObjectManager<TCompRender>();
  om_render->forEach([](TCompRender* c) {

    TCompTransform* c_transform = c->get<TCompTransform>();

    cb_object.obj_world = c_transform->asMatrix();
    //cb_object.obj_color = e->color
    cb_object.updateGPU();
    if (c->texture)
      c->texture->activate(0);
    c->tech->activate();
    c->mesh->activateAndRender();
  });

}
