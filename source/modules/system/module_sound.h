#pragma once

#include "modules/module.h"
#include "utils/variant.h"
#include <FMOD/fmod.hpp>
#include <FMOD/fmod_studio.hpp>


class CModuleSound : public IModule
{
public:
	CModuleSound(const std::string& name);
  bool start() override;
  bool stop() override;
  void update(float delta) override;

	FMOD_RESULT res;

private:

};
