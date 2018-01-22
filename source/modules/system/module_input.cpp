#include "mcv_platform.h"
#include "module_input.h"
#include "input/devices/keyboard.h"
#include "input/devices/mouse.h"
#include "input/devices/pad_xbox.h"


CModuleInput::CModuleInput(const std::string& name)
	: IModule(name)
{}

bool CModuleInput::start()
{
	static Input::CKeyboard keyboard("keyboard");
	static Input::CMouse mouse("mouse");
	static Input::CPadXbox pad("pad", 0);

	assignDevice(Input::PLAYER_1, &keyboard);
	assignDevice(Input::PLAYER_1, &mouse);
	assignDevice(Input::PLAYER_1, &pad);

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
	_registeredDevices.push_back(device);
}

Input::IDevice* CModuleInput::getDevice(const std::string& name)
{
	for (auto& dev : _registeredDevices)
	{
		if (dev->getName() == name)
		{
			return dev;
		}
	}
	return nullptr;
}

const Input::CHost& CModuleInput::host(Input::PlayerId playerId)
{
	return _hosts[static_cast<int>(playerId)];
}

const Input::TInterface_Keyboard& CModuleInput::keyboard() const
{
	return _hosts[Input::PLAYER_1].keyboard();
}

const Input::TInterface_Mouse& CModuleInput::mouse() const
{
	return _hosts[Input::PLAYER_1].mouse();
}

const Input::TInterface_Pad& CModuleInput::pad() const
{
	return _hosts[Input::PLAYER_1].pad();
}

const Input::TButton& CModuleInput::key(int keyIdx) const
{
	return keyboard().key(keyIdx);
}

const Input::TButton& CModuleInput::button(Input::EMouseButton bt) const
{
	return mouse().button(bt);
}

const Input::TButton& CModuleInput::button(Input::EPadButton bt) const
{
	return pad().button(bt);
}

void CModuleInput::feedback(const Input::TInterface_Feedback& data)
{
	_hosts[Input::PLAYER_1].feedback(data);
}

