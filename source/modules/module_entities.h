#pragma once

#include "modules/module.h"
#include "entity/entity.h"

class CModuleEntities : public IModule
{
public:
  CModuleEntities(const std::string& aname) : IModule(aname) { }
  void update(float delta) override;
  void render() override;
};

void loadEntities(const char* filename);
TEntity* getEntityByName(const char* name);