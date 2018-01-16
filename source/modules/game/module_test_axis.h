#pragma once

#include "modules/module.h"

class CModuleTestAxis : public IModule
{
public:
  bool start() override;
  void update(float delta) override;
  void render() override;
};