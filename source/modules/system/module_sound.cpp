#include "mcv_platform.h"
#include "module_sound.h"
#include "components/camera/comp_camera.h"

#pragma comment(lib, "fmod64_vc.lib")
#pragma comment(lib, "fmodstudio64_vc.lib")

using namespace FMOD;

CModuleSound::CModuleSound(const std::string& name)
	: IModule(name)
{}

bool CModuleSound::start()
{
	void *extraDriverData = NULL;

	System* lowLevelSystem = NULL;
	Studio::Bank* masterBank = NULL;		// ESTOS DOS SON OBLIGATORIOS, SON EL INDICE
	Studio::Bank* stringsBank = NULL;		// ESTOS DOS SON OBLIGATORIOS, SON EL INDICE
	//Studio::Bank* TorreBank = NULL;

	// arranco FMOD
	res = Studio::System::create(&system);    
	res = system->getLowLevelSystem(&lowLevelSystem);
    res = system->initialize(1024, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_3D_RIGHTHANDED, extraDriverData);

	// arranco los Bancos Básicos
	res = system->loadBankFile("./data/fmod/Master Bank.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &masterBank);
	res = system->loadBankFile("./data/fmod/Master Bank.strings.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &stringsBank);
    assert(res == FMOD_OK);

    // Carga eventos
    auto j = loadJson("data/fmod/sounds.json");
    for (auto& event : j["events"]) {
        Sound sound;
        sound.eventDescriptor = NULL;
        std::string event_src = event["src"].get<std::string>();
        std::string event_name = event["name"].get<std::string>();
        EngineSound.res = EngineSound.system->getEvent(event_src.c_str(), &sound.eventDescriptor);
        sound.positional = event.value("positional", false);
        sound.onStart = event.value("on_start", false);
        sound.eventInstance = NULL;
        EngineSound.res = sound.eventDescriptor->createInstance(&sound.eventInstance);
        events.insert(std::make_pair(event_name, sound));
        assert(res == FMOD_OK);
    }
  return true;
}

bool CModuleSound::stop()
{
  res = system->release();
  return true;
}

void CModuleSound::update(float delta)
{
    updateListenerAttributes();
		updatePositionalEvents();
		updateDelayedEvents(delta);
    system->update();
}

void CModuleSound::updateListenerAttributes() {
    if (!cameraHandle.isValid()) {
        CHandle cameraEntityHandle = EngineCameras.getActiveCamera();
        if (cameraEntityHandle.isValid()) {
            CEntity* cameraEntity = cameraEntityHandle;
            cameraHandle = cameraEntity->get<TCompCamera>();
        }
    }
    if (cameraHandle.isValid()) {
        TCompCamera* camera = cameraHandle;
        listenerAttributes.position = toFMODVector(camera->getPosition());
        listenerAttributes.forward = toFMODVector(camera->getFront());
        listenerAttributes.up = toFMODVector(camera->getUp());
        listenerAttributes.velocity = {};
    }
    else {
        listenerAttributes = {};
    }
    auto res = system->setListenerAttributes(0, &listenerAttributes);
}

void CModuleSound::updateDelayedEvents(float delta) {
	for (int i = 0; i < delayed_sounds.size(); ++i) {
		delayed_sounds[i].current_time += delta;
		if (delayed_sounds[i].current_time > delayed_sounds[i].time) {
			events[delayed_sounds[i].name].eventInstance->start();
			delayed_sounds.erase(delayed_sounds.begin() + i);
		}
	}
}

void CModuleSound::updatePositionalEvents() {
    for (auto& p : events) {
        auto& sound = p.second;
        if (sound.positional) {
           if (sound.hasTransform) {
               CHandle transform = sound.entity->get<TCompTransform>();
               TCompTransform* t = transform;
               FMOD_3D_ATTRIBUTES attributes = toFMODAttributes(*t);
               sound.eventInstance->set3DAttributes(&attributes);
           }
        }
    }
}

void CModuleSound::emitEvent(const std::string& sound) {
    events[sound].eventInstance->start();
}

void CModuleSound::emitPositionalEvent(const std::string& sound, const std::string& entityName) {
    CEntity* e = getEntityByName(entityName);
    events[sound].hasTransform = true;
    events[sound].entity = e;
    events[sound].eventInstance->start();
}

void CModuleSound::emitDelayedEvent(float time, std::string name) {
	DelayedSound sound;
	sound.time = time;
	sound.current_time = 0;
	sound.name = name;
	delayed_sounds.push_back(sound);
}


void CModuleSound::stopEvent(const std::string& sound) {
    events[sound].eventInstance->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT);
}

FMOD::Studio::System* CModuleSound::getSystem() {
    return system;
}

void CModuleSound::playInterior() {
    events["ambient"].eventInstance->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT);
    events["interior"].eventInstance->start();
}

void CModuleSound::playAmbient() {
    events["interior"].eventInstance->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT);
    events["ambient"].eventInstance->start();
}


FMOD_3D_ATTRIBUTES CModuleSound::toFMODAttributes(CTransform t) {
    FMOD_3D_ATTRIBUTES attr;
    attr.forward = toFMODVector(t.getFront());
    attr.velocity = FMOD_VECTOR();
    attr.position = toFMODVector(t.getPosition());
    attr.up = toFMODVector(t.getUp());
    return attr;
}

FMOD_VECTOR CModuleSound::toFMODVector(VEC3 v) {
    FMOD_VECTOR vec;
    vec.x = v.x;
    vec.y = v.y;
    vec.z = v.z;
    return vec;
}