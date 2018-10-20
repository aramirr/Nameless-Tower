#include "mcv_platform.h"
#include "gui_animate_uv.h"

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
			else {
				params->_minUV += _speed * delta;
				params->_maxUV += _speed * delta;
			}
      
    }
  }
}
