#pragma once

#include "modules/module.h"
#include "entity/entity.h"

class CModuleIA : public IModule
{

  //std::vector< IA >
public:
  void update(float delta) override;
  void render() override;
  IAIController* getNewAIControler(const json& j, TEntity* e);
};

