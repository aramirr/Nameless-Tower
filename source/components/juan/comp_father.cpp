#include "mcv_platform.h"
#include "comp_father.h"
#include "entity/entity.h"
#include "entity/entity_parser.h"
#include "components/juan/comp_transform.h"
#include "entity/common_msgs.h"

DECL_OBJ_MANAGER("father", TCompFather);

using namespace physx;

void TCompFather::load(const json& j, TEntityParseContext& ctx) {
  
}

void TCompFather::registerMsgs() {}



void TCompFather::debugInMenu() {
} 


void TCompFather::update(float dt) {}
