#pragma once

#include "modules/module.h"
#include "utils/variant.h"
#include <fmod.hpp>
#include <fmod_studio.hpp>

using namespace FMOD;

class CModuleSound : public IModule
{
public:
    struct Sound {
        FMOD::Studio::EventDescription* eventDescriptor = nullptr;
        FMOD::Studio::EventInstance* eventInstance;
        CEntity* entity;
        bool stopFadeOut = false;
        bool positional = false;
        bool onStart = false;
        bool hasTransform = false;
    };
		struct DelayedSound {
			float time;
			float current_time;
			std::string name;
		};

	CModuleSound(const std::string& name);
    bool start() override;
    bool stop() override;
    void update(float delta) override;

	FMOD_RESULT res;
	Studio::System* system = NULL;
    std::map<std::string, Sound> events;
	std::vector<std::string> compSounds;
    FMOD::Studio::System* getSystem();
    void stopEvent(FMOD::Studio::EventInstance * instance, bool fadeout = false);
    void playInterior();
    void playAmbient();
		void updatePositionalEvents();
		void updateDelayedEvents(float delta);
    void emitPositionalEvent(const std::string& sound, const std::string& entityName);
    void emitEvent(const std::string& sound);
    void emitDelayedEvent(float time, std::string name);
    void stopEvent(const std::string& sound);
		void setVolumen(float volumen);
		void registerCompSound(std::string name);
    FMOD_3D_ATTRIBUTES toFMODAttributes(CTransform t);
    FMOD_VECTOR toFMODVector(VEC3 v);
		
		std::vector<DelayedSound> delayed_sounds;
private:
    struct FollowingEvent {
        FMOD::Studio::EventInstance* eventInstance = nullptr;
        CHandle transformHandle;
    };
    void *extraDriverData = nullptr;
    FMOD::System* lowLevelSystem = nullptr;
    FMOD_3D_ATTRIBUTES listenerAttributes;
    CHandle cameraHandle;	

    void updateListenerAttributes();
 };
