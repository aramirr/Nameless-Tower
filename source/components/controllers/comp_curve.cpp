#include "mcv_platform.h"
#include "comp_curve.h"
#include "components/juan/comp_transform.h"
#include "geometry/curve.h"
#include "../camera/comp_camera_manager.h"

DECL_OBJ_MANAGER("curve_controller", TCompCurve);

void TCompCurve::debugInMenu() {
	static const char* curve_modes_str =
		"Linear\0"
		"Bounce\0"
		"Static\0"
		"\0";

	int nsegments = _curve->getKnotSize() - 1;
	float ratioPerSegment = 1.f / (float)nsegments;
	int currentSegment = (int)(_percentage / ratioPerSegment);
	float segmentRatio = fmodf(_percentage, ratioPerSegment) / ratioPerSegment;

	ImGui::Text("knots_size %i", _curve->getKnotSize());
	ImGui::Text("num segments %i", nsegments); 
	ImGui::Text("ratioPerSegment %f", ratioPerSegment); 
	ImGui::Text("current_segment %i", currentSegment);
	ImGui::Text("segmentRatio %f", segmentRatio);

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
 
//-----------------------------------------------------------------------
// GENERACION DE NUMEROS PSEUDOALEATORIOS
//-----------------------------------------------------------------------

#define MASK 2147483647
#define PRIME 65539
#define SCALE 0.4656612875e-9

int Seed = 26508293; // <--Introduzca aqui la semilla

#define Rand()  (( Seed = ( (Seed * PRIME) & MASK) ) * SCALE )

#define Randint(low,high) ( (int) (low + (high-(low)+1) * Rand()))

#define Randfloat(low,high) ( (low + (high-(low))*Rand()))


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
	CEntity* camera_manager = (CEntity*)getEntityByName("camera_manager");
	TCompCameraManager* cm = camera_manager->get<TCompCameraManager>();
	bool temblor = cm->getTemblor();
	bool temblor_flojo = cm->getTemblorFlojo();
	// actualizar la transform con la nueva posicion
	// evaluar curva con dicho ratio
	VEC3 pos = _curve->evaluateAsCatmull(_percentage, loop);
	TCompTransform* c_transform = get<TCompTransform>();
	c_transform->setPosition(pos);
	if (temblor) {



		//dbg("TEMBLOR\n");
		//CEntity* camera = (CEntity*)Engine.getCameras().getActiveCamera();
		/*std:string str = camera->getName();
		dbg(str.c_str());
		dbg("\n");*/
		//TCompTransform* c = camera->get<TCompTransform>();

		VEC3 newPos = c_transform->getPosition();
		VEC3 newFront = c_transform->getFront();
		VEC3 newLeft = c_transform->getLeft();

		float x = Randfloat(-0.1f, 0.1f);
		//dbg(("X: " + std::to_string(x) + "\n").c_str());
		float y = Randfloat(-0.025f, 0.025f);
		//dbg(("Y: " + std::to_string(y) + "\n").c_str());
		float z = Randfloat(-0.1f, 0.1f);
		//dbg(("Z: " + std::to_string(z) + "\n").c_str());

		newLeft *= x;
		newPos += newLeft;
		//newPos.x += x;
		//newFront.x += x;

		newPos.y += y;
		//newFront.y += y;

		//newPos.z += z;
		//newFront.z += z;

		c_transform->setPosition(newPos);
		c_transform->lookAt(newPos, newPos + newFront);
	}
	else if (temblor_flojo) {
		VEC3 newPos = c_transform->getPosition();
		VEC3 newFront = c_transform->getFront();
		VEC3 newLeft = c_transform->getLeft();

		float x = Randfloat(-0.025f, 0.025f);
		float y = Randfloat(-0.01f, 0.01f);
		float z = Randfloat(-0.025f, 0.025f);

		newLeft *= x;
		newPos += newLeft;

		newPos.y += y;

		c_transform->setPosition(newPos);
		c_transform->lookAt(newPos, newPos + newFront);
	}
	else {
		_target = getEntityByName(_targetName);

		if (_target.isValid())
		{
			c_transform->lookAt(pos, getTargetPos());
		}
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
