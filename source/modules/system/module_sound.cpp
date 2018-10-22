#include "mcv_platform.h"
#include "module_sound.h"
#include "render/render_objects.h"
#include "components/camera/comp_camera.h"
#include "components/juan/comp_transform.h"

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
        Studio::EventDescription* event_description = NULL;
        std::string event_src = event["src"].get<std::string>();
        std::string event_name = event["name"].get<std::string>();
        EngineSound.res = EngineSound.system->getEvent(event_src.c_str(), &event_description);

        Studio::EventInstance* event_instance = NULL;
        EngineSound.res = event_description->createInstance(&event_instance);
        events.insert(std::make_pair(event_name, event_instance));
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

void CModuleSound::emitEvent(const std::string& sound) {
    events[sound]->start();
}

void CModuleSound::stopEvent(const std::string& sound) {
    events[sound]->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT);
}

void CModuleSound::setVolumen(float volumen)
{
	std::map<std::string, Studio::EventInstance*>::iterator it;

	for (it = events.begin(); it != events.end(); it++)
	{
		it->second->setVolume(volumen);
	}

	TMsgVolumeSound msg;
	msg.volumen = volumen;

	for (int i = 0; i < compSounds.size(); i++) {
		CEntity* cs = getEntityByName(compSounds[i]);
		dbg(compSounds[i].c_str());
		dbg("\n");
		cs->sendMsg(msg);
	}
	
}

void CModuleSound::registerCompSound(std::string name)
{
	compSounds.push_back(name);
}

FMOD::Studio::System* CModuleSound::getSystem() {
    return system;
}

FMOD_3D_ATTRIBUTES CModuleSound::toFMODAttributes(CTransform t) {
    FMOD_3D_ATTRIBUTES attr;
    attr.forward = toFMODVector(t.getFront());
    attr.velocity = FMOD_VECTOR();
    attr.position = toFMODVector(t.getPosition());
    return attr;
}

FMOD_VECTOR CModuleSound::toFMODVector(VEC3 v) {
    FMOD_VECTOR vec;
    vec.x = v.x;
    vec.y = v.y;
    vec.z = v.z;
    return vec;
}

void CModuleSound::playInterior() {
    events["ambient"]->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT);
    events["interior"]->start();
}

void CModuleSound::playAmbient() {
    events["interior"]->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT);
    events["ambient"]->start();
}