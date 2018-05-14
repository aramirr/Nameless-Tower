#include "mcv_platform.h"
#include "comp_sound.h"
#include "resources/resources_manager.h"
#include "render/render_utils.h"
#include "components/juan/comp_transform.h"
#include "render/render_objects.h"


DECL_OBJ_MANAGER("sound", TCompSound);

using namespace FMOD;

TCompSound::TCompSound()
{  
}

TCompSound::~TCompSound() {
}

// ---------------------------------------------------------------------------------------
void TCompSound::load(const json& j, TEntityParseContext& ctx) {

	void *extraDriverData = NULL;

	Studio::System* system = NULL;
	System* lowLevelSystem = NULL;
	Studio::Bank* masterBank = NULL;		// ESTOS DOS SON OBLIGATORIOS, SON EL INDICE
	Studio::Bank* stringsBank = NULL;		// ESTOS DOS SON OBLIGATORIOS, SON EL INDICE
	Studio::Bank* TorreBank = NULL;

	// arranco FMOD
	res = Studio::System::create(&system);
	res = system->getLowLevelSystem(&lowLevelSystem);
	res = system->initialize(1024, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, extraDriverData);

	// arranco los Bancos Básicos
	res = system->loadBankFile("./data/Master Bank.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &masterBank);
	res = system->loadBankFile("./data/Master Bank.strings.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &stringsBank);

	// un par de bancos de ejemplo    
	res = system->loadBankFile("./data/StepBank.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &TorreBank);


	// Cargamos los EventDescriptors
	Studio::EventDescription* step_description = NULL;
	res = system->getEvent("event:/Steps/footsteps", &step_description); 
}

void TCompSound::update(float dt) {
 
}

void TCompSound::debugInMenu() {
  

}