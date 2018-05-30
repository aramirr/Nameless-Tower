#include "mcv_platform.h"
#include "entity/entity_parser.h"
#include "comp_particles.h"
#include "juan/comp_transform.h"
#include "entity/common_msgs.h"

DECL_OBJ_MANAGER("particles", TCompParticles);

void TCompParticles::debugInMenu() {}

void TCompParticles::registerMsgs() {
}


void TCompParticles::load(const json& j, TEntityParseContext& ctx) {
}

void TCompParticles::update(float dt) {}

