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
    FMOD::Studio::EventInstance* emitFollowingEvent(const std::string& sound, CHandle transformHandle);
    FMOD::Studio::EventInstance* emitEvent(const std::string& sound, const CTransform* transform);
    FMOD::Studio::EventInstance* emitEvent(const std::string& sound, const FMOD_3D_ATTRIBUTES* attributes = nullptr);
    FMOD::Studio::System* getSystem();
    void stopEvent(FMOD::Studio::EventInstance * instance, bool fadeout = false);
    void playInterior();
    void playAmbient();
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
    void updateFollowingEvents();
    FMOD_3D_ATTRIBUTES toFMODAttributes(CTransform t);
    FMOD_VECTOR toFMODVector(VEC3 v);
};
