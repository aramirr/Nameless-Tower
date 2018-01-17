#pragma once

#include "device.h"
#include "interfaces/interface_keyboard.h"

namespace Input
{
	class CHost
	{
	public:
		CHost() {}

		void update(float delta);
		void assignDevice(IDevice* device);

		const TInterface_Keyboard& keyboard() const { return _keyboard; }

	private:
		TInterface_Keyboard _keyboard;

		VDevices _devices;
	};
}
