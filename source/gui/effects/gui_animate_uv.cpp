#include "mcv_platform.h"
#include "gui_animate_uv.h"

#define MASK 2147483647
#define PRIME 65539
#define SCALE 0.4656612875e-9

int Seed = 26508293; // <--Introduzca aqui la semilla

#define Rand()  (( Seed = ( (Seed * PRIME) & MASK) ) * SCALE )

#define Randint(low,high) ( (int) (low + (high-(low)+1) * Rand()))

#define Randfloat(low,high) ( (low + (high-(low))*Rand()))

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
			else if (temblor) {
				float x = Randfloat(-10.f, 10.f);
				float y = Randfloat(-10.f, 10.f);;
				params->_minUV += _speed * delta * x;
				params->_maxUV += _speed * delta * y;
			}
			else {
				params->_minUV += _speed * delta;
				params->_maxUV += _speed * delta;
			}

		}
	}
}
