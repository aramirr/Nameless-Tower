#include "mcv_platform.h"
#include "host.h"

namespace Input
{

	void CHost::update(float delta)
	{
		for (auto& device : _devices)
		{
			device->updateKeyboardData(delta, _keyboard);
		}
	}

	void CHost::assignDevice(IDevice* device)
	{
		_devices.push_back(device);
	}

}
