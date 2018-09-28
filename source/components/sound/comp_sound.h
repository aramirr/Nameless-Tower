#ifndef INC_COMP_SOUND_H_
#define INC_COMP_SOUND_H_

#include "./components/comp_base.h"
#include "ctes.h"
#include "render/cte_buffer.h"
#include <stdio.h>
#include <conio.h>
#include <fmod.hpp>
#include <fmod_studio.hpp>
#include <string>
#include <map>

using namespace std;

struct TCompSound: public TCompBase {
    struct Sounds {
        std::string path;
        FMOD::Studio::EventDescription* eventDescriptor = nullptr;
        FMOD::Studio::EventInstance* eventInstance;
        bool stopFadeOut = false;
        bool positional = false;
        bool multiInstancing = true;
    };

    void load(const json& j, TEntityParseContext& ctx);
    void update(float dt);
    void debugInMenu();
    void playInterior();
    void playAmbient();
    void playSound(std::string name);
    void stopSound(std::string name);
    FMOD_3D_ATTRIBUTES toFMODAttributes(CTransform t);
    FMOD_VECTOR toFMODVector(VEC3 v);

    std::map<std::string, Sounds> events;
  DECL_SIBLING_ACCESS();
};

#endif
