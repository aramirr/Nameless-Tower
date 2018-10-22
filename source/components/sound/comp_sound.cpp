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
		Studio::EventDescription* event_description= NULL;
        std::string event_src = event["src"].get<std::string>();
        std::string event_name = event["name"].get<std::string>();
        FMOD_RESULT res = EngineSound.system->getEvent(event_src.c_str(), &event_description);
        assert(res == FMOD_OK);
		Studio::EventInstance* event_instance = NULL;
        res = event_description->createInstance(&event_instance);
        assert(res == FMOD_OK);
        positional = event.value("positional", false); 
        event_description->is3D(&positional);

        eventInstance = event_instance;
        eventDescriptor = event_description;
        
        multiInstancing = event.value("multi_instancing", false);
        stopFadeOut = event.value("stop_fade_out", false);
        onStart = event.value("on_start", false);

				
	}	
	CEntity* e = ctx.current_entity;
	assert(e);
	dbg(e->getName());
	dbg("\n");
	EngineSound.registerCompSound(e->getName());
}

void TCompSound::update(float dt) {
    TCompTransform* transform = get<TCompTransform>();	
	if (positional) {
		FMOD_3D_ATTRIBUTES attributes = toFMODAttributes(*transform);
		eventInstance->set3DAttributes(&attributes);
	}
}

void TCompSound::debugInMenu() {
}

void TCompSound::playSound(std::string name) {
    eventInstance->start();
    events.insert(std::make_pair(name, eventInstance));
}
void TCompSound::stopSound(std::string name) {
    events[name]->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT);
}

void TCompSound::onGroupCreated(const TMsgEntitiesGroupCreated& msg) {
    if (onStart)
        playSound("sound");
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
	
	std::map<std::string, Studio::EventInstance*>::iterator it;

	for (it = events.begin(); it != events.end(); it++)
	{
		it->second->setVolume(volumen);
		dbg("MUTE\n");
	}
}

void TCompSound::registerMsgs() {
    DECL_MSG(TCompSound, TMsgEntitiesGroupCreated, onGroupCreated);
    DECL_MSG(TCompSound, TMsgPlaySound, playSoundMsg);
		DECL_MSG(TCompSound, TMsgVolumeSound, setVolumen);
    
}