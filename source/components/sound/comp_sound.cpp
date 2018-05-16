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
		std::string event_name = event.get<std::string>();
		EngineSound.res = EngineSound.system->getEvent(event_name.c_str(), &event_description);

		Studio::EventInstance* event_instance = NULL;
		EngineSound.res = event_description->createInstance(&event_instance);
		events.push_back(event_instance);
	}	
}

void TCompSound::update(float dt) {
 
}

void TCompSound::debugInMenu() {
  

}