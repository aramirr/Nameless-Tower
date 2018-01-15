#pragma once

#include "modules/module.h"

class CModuleMainMenu : public IModule
{
public:
  bool start() override;
  void update(float delta) override;
};