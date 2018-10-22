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
				if (firstCredits) {
					params->_minUV = VEC2(0, 0.4f);
					params->_maxUV = VEC2(1, 1.4f);
					firstCredits = false;
				}
				else {
					params->_minUV += _speed * delta /** x*/;
					params->_maxUV += _speed * delta /** y*/;
				}
				//float x = Randfloat(-10.f, 10.f);
				//float y = Randfloat(-10.f, 10.f);
				
				
				if (params->_minUV.y >= 1.f) {
					HWND handle = ::FindWindowEx(0, 0, "MCVWindowsClass", 0);
					HCURSOR Cursor = LoadCursorFromFile("data/textures/gui/PaiCursor.cur"); //.cur or .ani
					SetCursor(Cursor);
					SetClassLongPtr(handle, GCLP_HCURSOR, reinterpret_cast<LONG_PTR>(Cursor));

					EngineUI.desactivateWidget("pantallaCreditos");
					EngineUI.activateWidget("pantallaFinal");
					EngineUI.activeFinalMenu();
				}
				dbg("CREDITOS: %f\n",params->_maxUV);
			}
			else {
				params->_minUV += _speed * delta;
				params->_maxUV += _speed * delta;
			}

		}
	}
}
