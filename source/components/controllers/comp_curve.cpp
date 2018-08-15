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
	ImGui::Checkbox("Loop", &loop);
	ImGui::Combo("CurveMode", &current_mode, curve_modes_str);
}

void TCompCurve::load(const json& j, TEntityParseContext& ctx) {

  std::string curve_name = j["curve"];
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
  loop = j.value("loop", true);
  _targetName = j.value("target", "");
  _target = getEntityByName(_targetName);
}
 
void TCompCurve::update(float DT)
{
  if (!_curve)
    return;

  // actualizar ratio
  if (_automove)
  {
	  if (current_mode != 2)
	  {
		  _percentage += _speed * DT * direction;
	  }

	  if (current_mode == 1)
	  {
		  if (_percentage >= 1.0f)
		  {
			  if (loop == false)
			  {
				  _automove = false;
			  }
			  else
			  {
				  direction = -1;
			  }
		  }
		  if (_percentage <= 0.0f)
		  {
			  if (loop == false)
			  {
				  _automove = false;
			  }
			  else
			  {
				  direction = 1;
			  }
		  }
	  }
	  else
	  {
		  if (_percentage >= 1.0f)
		  {
			  if (loop == false)
			  {
				  _automove = false;
			  }
			  else
			  {
				  _percentage = 0.0f;
			  }
		  }
	  }


	  // actualizar la transform con la nueva posicion
	  // evaluar curva con dicho ratio
	  VEC3 pos = _curve->evaluateAsCatmull(_percentage);
	  TCompTransform* c_transform = get<TCompTransform>();
	  c_transform->setPosition(pos);

	  _target = getEntityByName(_targetName);
 	  if (_target.isValid())
	  {
		  c_transform->lookAt(pos, getTargetPos());
	  }
  }
  else
  {
	  if (_percentage >= 1.0f)
	  {
		  if (loop == false)
		  {
			  _automove = false;
		  }
		  else
		  {
			  _percentage = 0.0f;
		  }
		
	  }
  }


  // actualizar la transform con la nueva posicion
  // evaluar curva con dicho ratio
  VEC3 pos = _curve->evaluateAsCatmull(_percentage);
  TCompTransform* c_transform = get<TCompTransform>();
  c_transform->setPosition(pos);
  _target = getEntityByName(_targetName);

  if (_target.isValid())
  {	  
	  c_transform->lookAt(pos, getTargetPos());
  }
}

VEC3 TCompCurve::getTargetPos()
{
  if (_target.isValid())
  {
    CEntity* e_target = _target;
    TCompTransform* c_transform = e_target->get<TCompTransform>();
    return c_transform->getPosition();
  }
  return VEC3::Zero;
}

void TCompCurve::activate() {
	_automove = true;
}

void TCompCurve::deactivate() {
	_automove = false;
}