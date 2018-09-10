#pragma once

#include "modules/module.h"
#include <lua\SLB\SLB.hpp>

using namespace SLB;

class CModuleLevel2: public IModule
{

  bool carga = true;
  bool pausa = false;
	CHandle h_e_camera;

public:
    CModuleLevel2(const std::string& aname) : IModule(aname) { }
  bool start() override;
  bool stop() override;
  void update(float delta) override;
  void render() override;

};

