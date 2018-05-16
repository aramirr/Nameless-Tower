#pragma once

#include "modules/module.h"
#include "utils/variant.h"
#include <fmod.hpp>
#include <fmod_studio.hpp>

using namespace FMOD;

class CModuleSound : public IModule
{
public:
	CModuleSound(const std::string& name);
  bool start() override;
  bool stop() override;
  void update(float delta) override;

	FMOD_RESULT res;
	Studio::System* system = NULL;

private:

};
