#include "mcv_platform.h"
#include "module_sound.h"
#include "render/render_objects.h"

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
	Studio::Bank* TorreBank = NULL;

	// arranco FMOD
	res = Studio::System::create(&system);
	res = system->getLowLevelSystem(&lowLevelSystem);
	res = system->initialize(1024, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, extraDriverData);

	// arranco los Bancos Básicos
	res = system->loadBankFile("./data/fmod/Master Bank.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &masterBank);
	res = system->loadBankFile("./data/fmod/Master Bank.strings.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &stringsBank);

	// un par de bancos de ejemplo    
	res = system->loadBankFile("./data/fmod/StepBank.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &TorreBank);


	// Cargamos los EventDescriptors
	//Studio::EventDescription* step_description = NULL;
	//res = system->getEvent("event:/Steps/footsteps", &step_description);
  return true;
}

bool CModuleSound::stop()
{
  return true;
}

void CModuleSound::update(float delta)
{
	system->update();
}
