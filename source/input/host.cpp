#include "mcv_platform.h"
#include "host.h"

namespace Input
{

	void CHost::update(float delta)
	{
		for (auto& device : _devices)
		{
			device->updateKeyboardData(delta, _keyboard);
			device->updateMouseData(delta, _mouse);
			device->updatePadData(delta, _pad);
		}
	}

	void CHost::assignDevice(IDevice* device)
	{
		_devices.push_back(device);
	}

	void CHost::feedback(const TInterface_Feedback& data)
	{
		for (auto& device : _devices)
		{
			device->feedback(data);
		}
	}

}
