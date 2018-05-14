#ifndef INC_COMP_SOUND_H_
#define INC_COMP_SOUND_H_

#include "./components/comp_base.h"
#include "ctes.h"
#include "render/cte_buffer.h"
#include <stdio.h>
#include <conio.h>
#include <FMOD/fmod.hpp>
#include <FMOD/fmod_studio.hpp>


struct TCompSound: public TCompBase {
	FMOD_RESULT res;

  TCompSound();
  ~TCompSound();
  void load(const json& j, TEntityParseContext& ctx);
  void update(float dt);
  void debugInMenu();

  DECL_SIBLING_ACCESS();
};

#endif
