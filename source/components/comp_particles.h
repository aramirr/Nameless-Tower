#pragma once

#include "components/comp_base.h"
#include "particles/particle_system.h"

struct TCompParticles : public TCompBase
{
  Particles::TCoreSystem* _core = nullptr;
  Particles::TParticlesHandle   _particles = 0;
  bool                          _launched = false;
  float                         _fadeOut = 0.f;
  std::string parent_name;

  TCompParticles() = default;
  static void registerMsgs();

  void load(const json& j, TEntityParseContext& ctx);
  void onCreated(const TMsgEntityCreated&);
  void onDestroyed(const TMsgEntityDestroyed&);
  void emit();
  void debugInMenu();
};

