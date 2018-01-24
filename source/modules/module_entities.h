#pragma once

#include "modules/module.h"
#include "entity/entity.h"

class CModuleEntities : public IModule
{
public:
  void update(float delta) override;
  void render() override;
};

void loadEntities(const char* filename);