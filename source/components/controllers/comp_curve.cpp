#include "mcv_platform.h"
#include "comp_curve.h"
#include "components/juan/comp_transform.h"
#include "geometry/curve.h"

DECL_OBJ_MANAGER("curve_controller", TCompCurve);



void TCompCurve::debugInMenu() {
	static const char* curve_modes_str =
		"Linear\0"
		"Bounce\0"
		"Static\0"
		"\0";

	ImGui::DragFloat("Percent", &_percentage, 0.01f, 0.001f, 1.f);
	ImGui::DragFloat("Speed", &_speed, 0.01f);
	ImGui::Checkbox("Enabled", &_automove);
	ImGui::Combo("CurveMode", &current_mode, curve_modes_str);
}

void TCompCurve::load(const json& j, TEntityParseContext& ctx) {

  curve_name = j.value("curve", "default_curve_name");
  _curve = Resources.get(curve_name)->as<CCurve>();
  _speed = j.value<float>("speed", 0.f);
  curve_mode = j.value("mode", "linear");
  if (curve_mode == "linear")
	  current_mode = 0;
  if (curve_mode == "bounce")
	  current_mode = 1;
  if (curve_mode == "static")
	  current_mode = 2;
  _automove = j.value("automove", true);
}

void TCompCurve::update(float dt)
{
  if (!_curve)
    return;

  // actualizar ratio
  if (_automove)
  {
	if (current_mode != 2)
	{
      _percentage += _speed * dt * direction;
	}
	
	if (current_mode == 1)
	{
		if (_percentage >= 1.0f)
		{
			direction = -1;
		}
		if (_percentage <= 0.0f)
		{
			direction = 1;
		}
	}
	else
	{
		if (_percentage >= 1.0f)
		{
			_percentage = 0.0f;
		}
	}
	

	// actualizar la transform con la nueva posicion
	// evaluar curva con dicho ratio
	VEC3 pos = _curve->evaluateAsCatmull(_percentage);
	TCompTransform* c_transform = get<TCompTransform>();
	c_transform->setPosition(pos);
  }
  
}
