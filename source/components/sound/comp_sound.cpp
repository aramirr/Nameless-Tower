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
		Studio::EventDescription* event_description= NULL;
        std::string event_src = event["src"].get<std::string>();
        std::string event_name = event["name"].get<std::string>();
		EngineSound.res = EngineSound.system->getEvent(event_src.c_str(), &event_description);

		Studio::EventInstance* event_instance = NULL;
		EngineSound.res = event_description->createInstance(&event_instance);
		events.insert(std::make_pair(event_name, event_instance));
	}	
}

void TCompSound::update(float dt) {
}

void TCompSound::debugInMenu() {
}

void TCompSound::playSound(std::string name) {
    events[name]->start();
}
void TCompSound::stopSound(std::string name) {
    events[name]->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT);
}