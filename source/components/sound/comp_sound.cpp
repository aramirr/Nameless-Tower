#include "mcv_platform.h"
#include "comp_sound.h"
#include "resources/resources_manager.h"
#include "render/render_utils.h"
#include "components/juan/comp_transform.h"
#include "render/render_objects.h"
#include "entity/entity_parser.h"


DECL_OBJ_MANAGER("sound", TCompSound);

using namespace FMOD;

void TCompSound::load(const json& j, TEntityParseContext& ctx) {
	for (auto& event : j["events"]) {
        Sound sound;
		Studio::EventDescription* event_description= NULL;
        std::string event_src = event["src"].get<std::string>();
        std::string event_name = event["name"].get<std::string>();
        FMOD_RESULT res = EngineSound.system->getEvent(event_src.c_str(), &event_description);
        assert(res == FMOD_OK);
		Studio::EventInstance* event_instance = NULL;
        res = event_description->createInstance(&event_instance);
        assert(res == FMOD_OK);
        sound.positional = event.value("positional", false); 
        event_description->is3D(&sound.positional);

        sound.eventInstance = event_instance;
        sound.eventDescriptor = event_description;
        
        sound.stopFadeOut = event.value("stop_fade_out", false);
        sound.onStart = event.value("on_start", false);
        events.insert(std::make_pair(event_name, sound));
	}	
	CEntity* e = ctx.current_entity;
	assert(e);
	//dbg(e->getName());
	//dbg("\n");
	EngineSound.registerCompSound(e);
}

void TCompSound::update(float dt) {
    TCompTransform* transform = get<TCompTransform>();	
    for (auto& p : events) {
        auto& sound = p.second;
        if (sound.positional) {
            FMOD_3D_ATTRIBUTES attributes = toFMODAttributes(*transform);
            sound.eventInstance->set3DAttributes(&attributes);
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

void TCompSound::onGroupCreated(const TMsgEntitiesGroupCreated& msg) {
    for (auto& p : events) {
        auto& sound = p.second;
        if (sound.onStart)
            playSound(p.first);
    }
}

FMOD_3D_ATTRIBUTES TCompSound::toFMODAttributes(CTransform t) {
    FMOD_3D_ATTRIBUTES attr;
    attr.forward = toFMODVector(t.getFront());
    attr.velocity = FMOD_VECTOR();
    attr.position = toFMODVector(t.getPosition());
    attr.up = toFMODVector(t.getUp());
    return attr;
}

FMOD_VECTOR TCompSound::toFMODVector(VEC3 v) {
    FMOD_VECTOR vec;
    vec.x = v.x;
    vec.y = v.y;
    vec.z = v.z;
    return vec;
}

void TCompSound::playSoundMsg(const TMsgPlaySound& msg) {
    playSound("sound");
}

void TCompSound::setVolumen(const TMsgVolumeSound& msg) {
	float volumen = msg.volumen;
	
	std::map<std::string, Sound>::iterator it;

	for (it = events.begin(); it != events.end(); it++)
	{
		it->second.eventInstance->setVolume(volumen);
		//dbg("MUTE\n");
	}
}

void TCompSound::registerMsgs() {
    DECL_MSG(TCompSound, TMsgEntitiesGroupCreated, onGroupCreated);
    DECL_MSG(TCompSound, TMsgPlaySound, playSoundMsg);
		DECL_MSG(TCompSound, TMsgVolumeSound, setVolumen);
    
}