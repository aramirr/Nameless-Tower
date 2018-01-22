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
	Input::IDevice* getDevice(const std::string& name);

	void feedback(const Input::TInterface_Feedback& data);

	const Input::CHost& host(Input::PlayerId playerId);
	const Input::TInterface_Keyboard& keyboard() const;
	const Input::TInterface_Mouse& mouse() const;
	const Input::TInterface_Pad& pad() const;
	const Input::TButton& key(int keyIdx) const;
	const Input::TButton& button(Input::EMouseButton bt) const;
	const Input::TButton& button(Input::EPadButton bt) const;

private:
	Input::CHost _hosts[Input::NUM_PLAYERS];
	std::vector<Input::IDevice*> _registeredDevices;
};