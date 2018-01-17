#pragma once

#include "mcv_platform.h"
#include "interfaces/interface_keyboard.h"

namespace Input
{
	class IDevice
	{
	public:
		IDevice() {}

		virtual void updateKeyboardData(float delta, TInterface_Keyboard& data) { (void)delta;  (void)data; }

	private:
	};

	using VDevices = std::vector<IDevice*>;
}
