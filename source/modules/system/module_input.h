#pragma once

#include "modules/module.h"
#include "input/enums.h"
#include "input/host.h"

namespace Input
{
	class IDevice;
}

class CModuleInput : public IModule
{
public:
	CModuleInput(const std::string& name);
  bool start() override;
  bool stop() override;
  void update(float delta) override;

	void assignDevice(int hostIdx, Input::IDevice* device);

	const Input::CHost& host(Input::PlayerId playerId);

private:
	Input::CHost _hosts[Input::NUM_PLAYERS];
};