#include "mcv_platform.h"
#include "module_input.h"
#include "input/devices/keyboard.h"
#include "input/devices/mouse.h"
#include "input/devices/pad_xbox.h"
#include "input/mapping.h"
#include "utils/json.hpp"
#include <fstream>

// for convenience
using json = nlohmann::json;


CModuleInput::CModuleInput(const std::string& name)
	: IModule(name)
{}

bool CModuleInput::start()
{
	loadButtonDefinitions("data/keyboard_keys.json");

	static Input::CKeyboard keyboard("keyboard");
	static Input::CMouse mouse("mouse");
	static Input::CPadXbox pad("pad", 0);
	static Input::CMapping mapping("data/mapping.json");

	assignDevice(Input::PLAYER_1, &keyboard);
	assignDevice(Input::PLAYER_1, &mouse);
	assignDevice(Input::PLAYER_1, &pad);
	assignMapping(Input::PLAYER_1, &mapping);

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

void CModuleInput::assignMapping(int hostIdx, Input::CMapping* mapping)
{
	_hosts[hostIdx].assignMapping(mapping);
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

const Input::CHost& CModuleInput::host(Input::PlayerID playerId) const
{
	return _hosts[playerId];
}

const Input::CHost& CModuleInput::operator[](Input::PlayerID playerId) const
{
	return host(playerId);
}

void CModuleInput::feedback(const Input::TInterface_Feedback& data)
{
	_hosts[Input::PLAYER_1].feedback(data);
}

const Input::TInterface_Keyboard& CModuleInput::keyboard() const
{
	return host(Input::PLAYER_1).keyboard();
}

const Input::TInterface_Mouse& CModuleInput::mouse() const
{
	return host(Input::PLAYER_1).mouse();
}

const Input::TInterface_Pad& CModuleInput::pad() const
{
	return host(Input::PLAYER_1).pad();
}

const Input::TInterface_Mapping& CModuleInput::mapping() const
{
	return host(Input::PLAYER_1).mapping();
}

const Input::TButton& CModuleInput::operator[](Input::KeyID key) const
{
	return keyboard().key(key);
}

const Input::TButton& CModuleInput::operator[](Input::EMouseButton bt) const
{
	return mouse().button(bt);
}

const Input::TButton& CModuleInput::operator[](Input::EPadButton bt) const
{
	return pad().button(bt);
}

const Input::TButton& CModuleInput::operator[](const std::string& name) const
{
	return mapping().button(name);
}

void CModuleInput::loadButtonDefinitions(const std::string& filename)
{
	std::ifstream file_json(filename);
	json json_data;
	file_json >> json_data;

	// parse keyboard keys
	int i = 0;
	for (auto& btName : json_data)
	{
		const std::string& str = btName.get<std::string>();
		_buttonDefinitions[str] = Input::TButtonDef({ Input::EInterface::KEYBOARD, i++ });
	}

	// add mouse buttons
	_buttonDefinitions["mouse_left"] = Input::TButtonDef({ Input::EInterface::MOUSE, Input::MOUSE_LEFT });
	_buttonDefinitions["mouse_right"] = Input::TButtonDef({ Input::EInterface::MOUSE, Input::MOUSE_RIGHT });
	_buttonDefinitions["mouse_middle"] = Input::TButtonDef({ Input::EInterface::MOUSE, Input::MOUSE_MIDDLE });

	// add pad buttons
	_buttonDefinitions["pad_a"] = Input::TButtonDef({ Input::EInterface::PAD, Input::PAD_A });
	_buttonDefinitions["pad_b"] = Input::TButtonDef({ Input::EInterface::PAD, Input::PAD_B });
	_buttonDefinitions["pad_x"] = Input::TButtonDef({ Input::EInterface::PAD, Input::PAD_X });
	_buttonDefinitions["pad_y"] = Input::TButtonDef({ Input::EInterface::PAD, Input::PAD_Y });
	_buttonDefinitions["pad_l1"] = Input::TButtonDef({ Input::EInterface::PAD, Input::PAD_L1 });
	_buttonDefinitions["pad_l2"] = Input::TButtonDef({ Input::EInterface::PAD, Input::PAD_L2 });
	_buttonDefinitions["pad_l3"] = Input::TButtonDef({ Input::EInterface::PAD, Input::PAD_L3 });
	_buttonDefinitions["pad_r1"] = Input::TButtonDef({ Input::EInterface::PAD, Input::PAD_R1 });
	_buttonDefinitions["pad_r2"] = Input::TButtonDef({ Input::EInterface::PAD, Input::PAD_R2 });
	_buttonDefinitions["pad_r3"] = Input::TButtonDef({ Input::EInterface::PAD, Input::PAD_R3 });
	_buttonDefinitions["pad_left"] = Input::TButtonDef({ Input::EInterface::PAD, Input::PAD_LEFT });
	_buttonDefinitions["pad_right"] = Input::TButtonDef({ Input::EInterface::PAD, Input::PAD_RIGHT });
	_buttonDefinitions["pad_up"] = Input::TButtonDef({ Input::EInterface::PAD, Input::PAD_UP });
	_buttonDefinitions["pad_down"] = Input::TButtonDef({ Input::EInterface::PAD, Input::PAD_DOWN });
	_buttonDefinitions["pad_start"] = Input::TButtonDef({ Input::EInterface::PAD, Input::PAD_START });
	_buttonDefinitions["pad_options"] = Input::TButtonDef({ Input::EInterface::PAD, Input::PAD_OPTIONS });
	_buttonDefinitions["pad_lanalogx"] = Input::TButtonDef({ Input::EInterface::PAD, Input::PAD_LANALOG_X });
	_buttonDefinitions["pad_lanalogy"] = Input::TButtonDef({ Input::EInterface::PAD, Input::PAD_LANALOG_Y });
	_buttonDefinitions["pad_ranalogx"] = Input::TButtonDef({ Input::EInterface::PAD, Input::PAD_RANALOG_X });
	_buttonDefinitions["pad_ranalogy"] = Input::TButtonDef({ Input::EInterface::PAD, Input::PAD_RANALOG_Y });
}

const Input::TButtonDef* CModuleInput::getButtonDefinition(const std::string& name) const
{
	auto& btDef = _buttonDefinitions.find(name);
	if (btDef != _buttonDefinitions.end())
	{
		return &btDef->second;
	}
	return nullptr;
}

