#pragma once

#include "modules/module.h"
#include "utils/variant.h"
#include <fmod.hpp>
#include <fmod_studio.hpp>

using namespace FMOD;

class CModuleSound : public IModule
{
public:
	CModuleSound(const std::string& name);
    bool start() override;
    bool stop() override;
    void update(float delta) override;

	FMOD_RESULT res;
	Studio::System* system = NULL;
    std::map<std::string, Studio::EventInstance*> events;
    FMOD::Studio::System* getSystem();
    void stopEvent(FMOD::Studio::EventInstance * instance, bool fadeout = false);
    void playInterior();
    void playAmbient();
    void emitEvent(const std::string& sound);
    void stopEvent(const std::string& sound);
private:
    struct FollowingEvent {
        FMOD::Studio::EventInstance* eventInstance = nullptr;
        CHandle transformHandle;
    };
    void *extraDriverData = nullptr;
    FMOD::System* lowLevelSystem = nullptr;
    FMOD_3D_ATTRIBUTES listenerAttributes;
    CHandle cameraHandle;

    std::vector<FollowingEvent> followingEvents;

    void updateListenerAttributes();
    FMOD_3D_ATTRIBUTES toFMODAttributes(CTransform t);
    
    FMOD_VECTOR toFMODVector(VEC3 v);
};
