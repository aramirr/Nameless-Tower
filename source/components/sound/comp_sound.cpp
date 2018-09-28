#include "mcv_platform.h"
#include "comp_sound.h"
#include "resources/resources_manager.h"
#include "render/render_utils.h"
#include "components/juan/comp_transform.h"
#include "render/render_objects.h"


DECL_OBJ_MANAGER("sound", TCompSound);

using namespace FMOD;

void TCompSound::load(const json& j, TEntityParseContext& ctx) {
	for (auto& event : j["events"]) {
        Sounds sound;        
		Studio::EventDescription* event_description= NULL;
        std::string event_src = event["src"].get<std::string>();
        std::string event_name = event["name"].get<std::string>();
		EngineSound.res = EngineSound.system->getEvent(event_src.c_str(), &event_description);
		Studio::EventInstance* event_instance = NULL;
		EngineSound.res = event_description->createInstance(&event_instance);

        sound.path = event_src;
        sound.eventInstance = event_instance;
        sound.eventDescriptor = event_description;
        sound.following = event.value("following", sound.following);
        sound.multiInstancing = event.value("multi_instancing", sound.multiInstancing);
        sound.stopFadeOut = event.value("stop_fade_out", sound.stopFadeOut);
		events.insert(std::make_pair(event_name, sound));
	}	
}

void TCompSound::update(float dt) {
    TCompTransform* transform = get<TCompTransform>();

	for (auto& p : events) {
		auto& sound = p.second;
		auto& eventInstance = sound.eventInstance;		
		if (sound.following) {
			FMOD_3D_ATTRIBUTES attributes = toFMODAttributes(*transform);
			eventInstance->set3DAttributes(&attributes);
		}
	}
}

void TCompSound::debugInMenu() {
}

void TCompSound::playSound(std::string name) {
    events[name].eventInstance->start();
}
void TCompSound::stopSound(std::string name) {
    events[name].eventInstance->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT);
}


FMOD_3D_ATTRIBUTES TCompSound::toFMODAttributes(CTransform t) {
    FMOD_3D_ATTRIBUTES attr;
    attr.forward = toFMODVector(t.getFront());
    attr.velocity = FMOD_VECTOR();
    attr.position = toFMODVector(t.getPosition());
    return attr;
}

FMOD_VECTOR TCompSound::toFMODVector(VEC3 v) {
    FMOD_VECTOR vec;
    vec.x = v.x;
    vec.y = v.y;
    vec.z = v.z;
    return vec;
}