#pragma once

#include "device.h"
#include "interfaces/interface_keyboard.h"
#include "interfaces/interface_mouse.h"
#include "interfaces/interface_pad.h"
#include "interfaces/interface_feedback.h"

namespace Input
{
	class CHost
	{
	public:
		CHost() {}

		void update(float delta);
		void assignDevice(IDevice* device);

		const TInterface_Keyboard& keyboard() const { return _keyboard; }
		const TInterface_Mouse& mouse() const { return _mouse; }
		const TInterface_Pad& pad() const { return _pad; }

		void feedback(const TInterface_Feedback& data);

	private:
		TInterface_Keyboard _keyboard;
		TInterface_Mouse _mouse;
		TInterface_Pad _pad;

		VDevices _devices;
	};
}
