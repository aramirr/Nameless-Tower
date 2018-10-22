#include "mcv_platform.h"
#include "gui_animate_uv.h"
#include "components\camera\comp_camera_manager.h"

namespace GUI
{
	void CAnimateUV::update(float delta)
	{
		TImageParams* params = _widget->getImageParams();
		if (params)
		{
			if (splash) {
				params->_minUV += _speed * delta;
				params->_maxUV -= _speed * delta;
			}
			else if (creditos) {
				//float x = Randfloat(-10.f, 10.f);
				//float y = Randfloat(-10.f, 10.f);
				params->_minUV += _speed * delta /** x*/;
				params->_maxUV += _speed * delta /** y*/;
				
				dbg("CREDITOS: %f\n",params->_maxUV);
			}
			else {
				params->_minUV += _speed * delta;
				params->_maxUV += _speed * delta;
			}

		}
	}
}
