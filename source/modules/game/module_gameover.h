#pragma once

#include "modules/module.h"

class CModuleGameOver : public IModule
{
public:
  bool start() override;
  void update(float delta) override;
};