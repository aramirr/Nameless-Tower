#pragma once

#include "modules/module_manager.h"
#include "modules/module_render.h"

class CEngine
{
public:
  static CEngine& get();

  bool start();
  bool stop();
  void update(float delta);
  void render();

  CModuleManager& getModules() { return _modules; }
  CModuleRender& getRender() { return _module_render; }
  //CModuleInput& getInput() { return _module_input; }

private:
  CModuleManager _modules;
  CModuleRender _module_render;
};