#include "mcv_platform.h"
#include "module_timer.h"


bool CModuleTimer::start()
{
	time_slower = 1;
	return true;
}

void CModuleTimer::update(float delta)
{
}

void CModuleTimer::render()
{
}

const float CModuleTimer::getTimeSlower()
{
	return time_slower;
}

const void CModuleTimer::setTimeSlower(float time) {
	time_slower = time;
}