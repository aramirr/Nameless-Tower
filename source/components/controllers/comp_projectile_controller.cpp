#include "mcv_platform.h"
#include "entity/entity_parser.h"
#include "comp_projectile_controller.h"
#include "components/juan/comp_transform.h"

using namespace physx;

DECL_OBJ_MANAGER("projectile_controller", TCompProjectileController);

void TCompProjectileController::debugInMenu() {
}

void TCompProjectileController::setEntity(CHandle new_entity) {
	h_entity = new_entity;
	assert(h_entity.isValid());
}

void TCompProjectileController::load(const json& j, TEntityParseContext& ctx) {
	setEntity(ctx.current_entity);
  time_interval = j.value("time_interval", 2.0f);
	is_active = true;
	current_interval = 0;
}

void TCompProjectileController::update(float dt) {
	if (is_active) {
		CEntity *e = h_entity;
		if (!h_entity.isValid()) return;
		current_interval += dt;
		if (current_interval > time_interval) {
			TEntityParseContext ctx;
			ctx.entity_starting_the_parse = CHandle(this).getOwner();
			ctx.root_transform = *(TCompTransform*)get<TCompTransform>();
			if (parseScene("data/prefabs/projectile.prefab", ctx)) {
				assert(!ctx.entities_loaded.empty());
			}
			current_interval = 0;
		}
	}	
}

void TCompProjectileController::registerMsgs() {
	DECL_MSG(TCompProjectileController, TMsgActivateProjectile, activate);
	DECL_MSG(TCompProjectileController, TMsgDeactivateProjectile, deactivate);
}

void TCompProjectileController::activate(const TMsgActivateProjectile& msg) {
	is_active = true;
}

void TCompProjectileController::deactivate(const TMsgDeactivateProjectile& msg) {
	is_active = false;
}