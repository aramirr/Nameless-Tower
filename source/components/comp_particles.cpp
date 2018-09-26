#include "mcv_platform.h"
#include "comp_particles.h"
#include "resources/resources_manager.h"
#include "particles/particle_system.h"
#include "particles\particle_parser.h"

DECL_OBJ_MANAGER("particles", TCompParticles);

void TCompParticles::registerMsgs()
{
  DECL_MSG(TCompParticles, TMsgEntityCreated, onCreated);
  DECL_MSG(TCompParticles, TMsgEntityDestroyed, onDestroyed);
}

void TCompParticles::debugInMenu()
{
  if (_core)
  {
    ImGui::Text("Core: %s", _core->getName().c_str());
	if (ImGui::TreeNode("Life")) {
		ImGui::DragFloat("Duration", &_core->life.duration, 0.1f, 0.f, 100.f);
		ImGui::DragFloat("Duration Variation", &_core->life.durationVariation, 0.1f, 0.f, 5.f);
		ImGui::DragInt("Max Particles", &_core->life.maxParticles, 0.1f, 0, 500);
		ImGui::DragFloat("Time Factor", &_core->life.timeFactor, 0.1f, 0.f, 2.f);

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Emission")) {
		ImGui::DragFloat("Interval", &_core->emission.interval, 0.1f, 0.f, 10.f);
		ImGui::DragInt("Count", &_core->emission.count, 0.1f, 0, 100);
		ImGui::DragFloat("Size", &_core->emission.size, 0.1f, 0.f, 20.f);
		ImGui::DragFloat("Size2", &_core->emission.size_2, 0.1f, 0.f, 20.f);
		ImGui::Text("On: %s", &_core->emission.on ? "Si" : "No");
    float aux_angle = rad2deg(_core->emission.angle);
		ImGui::DragFloat("Angle", &aux_angle, 0.1f, 0.f, 360.f);
    _core->emission.angle = deg2rad(aux_angle);

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Movement")) {
		ImGui::DragFloat("Velocity", &_core->movement.velocity, 0.1f, -11.f, 10.f);
		ImGui::DragFloat("Acceleration", &_core->movement.acceleration, 0.1f, -10.f, 10.f);
		ImGui::DragFloat("Spin", &_core->movement.spin, 0.1f, -50.f, 50.f);
		ImGui::DragFloat("Gravity", &_core->movement.gravity, 0.1f, 0.f, 1.f);
    ImGui::DragFloat("Wind", &_core->movement.wind, 0.1f, 0.f, 1.f);
    ImGui::DragFloat("Ground_y", &_core->movement.ground_y, 0.1f, 0.f, 100.f);

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Size")) {
		ImGui::DragFloat("Scale", &_core->size.scale, 0.1f, 0.f, 10.f);
		ImGui::DragFloat("Scale Variation", &_core->size.scale_variation, 0.1f, 0.f, 10.f);

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Color")) {
		ImGui::DragFloat("Opacity", &_core->color.opacity, 0.1f, 0.f, 1.f);

		ImGui::TreePop();
	}


  }
  ImGui::LabelText("Particles handle", "%d", _particles);
}

void TCompParticles::load(const json& j, TEntityParseContext& ctx)
{
  _fadeOut = j.value("fade_out", 0.f);
  auto& particlesName = j.value("core", "");
  //_core = Resources.get(particlesName)->as<Particles::TCoreSystem>();

  Particles::CParser parser;
  _core = parser.parseParticlesFile(particlesName);
  assert(_core);
  parent_name = j.value("parent_name", "");
  _core->parent_name = parent_name;
}

void TCompParticles::onCreated(const TMsgEntityCreated&)
{
  if (_core && !_particles )
  {
    _particles = Engine.getParticles().launchSystem(_core, CHandle(this).getOwner());
  }
}

void TCompParticles::onDestroyed(const TMsgEntityDestroyed&)
{
  if (_particles)
  {
    Engine.getParticles().kill(_particles, _fadeOut);
  }
}

void TCompParticles::emit() {
  if (_core)
  {
    _particles = Engine.getParticles().launchSystem(_core, CHandle(this).getOwner());
		_core->emission.on = true;
  }
}

void TCompParticles::start_emiter() {
	if (_core)
	{
		_core->emission.on = true;
	}
}

void TCompParticles::stop_emiter() {
	if (_core)
	{
		_core->emission.on = false;
	}
}
