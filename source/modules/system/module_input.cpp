#include "mcv_platform.h"
#include "module_input.h"
#include "input/devices/keyboard.h"


CModuleInput::CModuleInput(const std::string& name)
	: IModule(name)
{}

bool CModuleInput::start()
{
	static Input::CKeyboard keyboard;

	assignDevice(static_cast<int>(Input::PlayerId::PLAYER_1), &keyboard);

  return true;
}

bool CModuleInput::stop()
{
  return true;
}

void CModuleInput::update(float delta)
{
	for (auto& host : _hosts)
	{
		host.update(delta);
	}
}

void CModuleInput::assignDevice(int hostIdx, Input::IDevice* device)
{
	_hosts[hostIdx].assignDevice(device);
}

const Input::CHost& CModuleInput::host(Input::PlayerId playerId)
{
	return _hosts[static_cast<int>(playerId)];
}
