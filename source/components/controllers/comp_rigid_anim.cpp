#include "mcv_platform.h"
#include "comp_rigid_anim.h"
#include "components/juan/comp_transform.h"
#include "entity/entity_parser.h"

DECL_OBJ_MANAGER("rigid_anim", TCompRigidAnim);

void TCompRigidAnim::load(const json& j, TEntityParseContext& ctx) {
	father = j.value("father", false);
	if (!father) {
		controller.track_name = j.value("track", "");
		if (controller.track_name.empty()) {
			CEntity* e = ctx.current_entity;
			assert(e);
			controller.track_name = e->getName();
		}
		assert(j.count("src") > 0);
		controller.anims = Resources.get(j["src"])->as<RigidAnims::CRigidAnimResource>();
		controller.track_index = controller.anims->findTrackIndexByName(controller.track_name);
		assert(controller.track_index != RigidAnims::CController::invalid_track_index);
		current_time = 0;
		speed_factor = j.value("speed_factor", 1.0f);
		loops = j.value("loops", true);
	}

	
}

void TCompRigidAnim::debugInMenu() {
  ImGui::DragFloat("Time", &current_time, 0.01f, 0.f, 10.0f);
  ImGui::DragFloat("Speed Factor", &speed_factor, 0.01f, 0.f, 5.0f);
  ImGui::Checkbox("Loops", &loops);
}

void TCompRigidAnim::update(float dt) {
    
  if (!is_moving)
    return;

	timer += dt;
	if (timer >= wait_time) {
		TCompTransform* c_trans = get< TCompTransform >();
		if (!father && c_trans != nullptr) {
			// Sample the animation in the current time
			RigidAnims::TKey k;
			
			dbg("JOSUE %s\n", name.c_str());
			bool has_finished = controller.sample(&k, current_time);

			// Transfer the key data to the comp transform
			c_trans->setPosition(k.pos);
			c_trans->setRotation(k.rot);
			c_trans->setScale(k.scale);

			if (has_finished) {
				if (loops)
					current_time = 0;
				else is_moving = false;
				// loop, change direction?, set is_moving = false...
			}
		}
		else {
			for (int i = 0; i < sons.size(); i++) {
				dbg("JONAS %i --%i\n", i, sons.size());
				CEntity* entity = (CEntity*)getEntityByName(sons[i]);
				TMsgActivateAnim msg;
				msg.name = sons[i];
				entity->sendMsg(msg);
			}
		}
	}

  // Advance the time
  current_time += dt * speed_factor;
}

void TCompRigidAnim::onGroupCreated(const TMsgEntitiesGroupCreated& msg) {
    TCompTransform* transform = get<TCompTransform>();
    controller.setInitialTransform(transform);
}

void TCompRigidAnim::activate(const TMsgActivateAnim& msg) {
	is_moving = true;
	name = msg.name;
	wait_time = msg.wait_time;
	timer = 0.f;
}

void TCompRigidAnim::desactivate(const TMsgDesactivateAnim & msg)
{
	is_moving = false;
	timer = 0.f;
	if (!father) {
		current_time = 0;
		RigidAnims::TKey k;
		TCompTransform* c_trans = get< TCompTransform >();
		controller.sample(&k, current_time);

		c_trans->setPosition(k.pos);
		c_trans->setRotation(k.rot);
		c_trans->setScale(k.scale);
	}
	else {
		for (int i = 0; i < sons.size(); i++) {
			CEntity* entity = (CEntity*)getEntityByName(sons[i]);
			TMsgDesactivateAnim msg;
			entity->sendMsg(msg);
		}
	}
}

void TCompRigidAnim::registerSon(const TMsgRegisterDestoyableSon & msg)
{
	if (father)
		sons.push_back(msg.name);
}

void TCompRigidAnim::registerMsgs() {
	DECL_MSG(TCompRigidAnim, TMsgEntitiesGroupCreated, onGroupCreated);
	DECL_MSG(TCompRigidAnim, TMsgActivateAnim, activate);
	DECL_MSG(TCompRigidAnim, TMsgDesactivateAnim, desactivate);
	DECL_MSG(TCompRigidAnim, TMsgRegisterDestoyableSon, registerSon);
}