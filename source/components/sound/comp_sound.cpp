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

	
}

void TCompSound::update(float dt) {
 
}

void TCompSound::debugInMenu() {
  

}