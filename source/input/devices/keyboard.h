#pragma once

#include "input/device.h"

namespace Input
{
	class CKeyboard : public IDevice
	{
		virtual void updateKeyboardData(float delta, TInterface_Keyboard& data) override;
	};
}