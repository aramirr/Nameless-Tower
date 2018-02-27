#include "mcv_platform.h"
#include "handle/handle.h"
#include "ai_destoyable.h"
#include "entity/entity_parser.h"
#include "components/comp_transform.h"
#include "render/render_utils.h"

DECL_OBJ_MANAGER("ai_destroyable", CAIDestroyable);

void CAIDestroyable::Init()
{
  // insert all states in the map
  AddState("idle", (statehandler)&CAIDestroyable::IdleState);
  AddState("trigger_destroy", (statehandler)&CAIDestroyable::TriggerDestroyState);
  AddState("destroy", (statehandler)&CAIDestroyable::DestoyState);

  // reset the state
  ChangeState("idle");
}

void CAIDestroyable::debugInMenu() {

  IAIController::debugInMenu();
  CEntity *c_trigger = (CEntity *)getEntityByName(trigger_actor);
  if (!c_trigger)
	  return;

  TCompTransform *mypos = getMyTransform();
  TCompTransform *ppos = c_trigger->get<TCompTransform>();
     
      ImGui::Text("Distance %f", VEC3::Distance(mypos->getPosition(), ppos->getPosition()));
	  ImGui::Text("acum_time %f", acum_time);
      
  
}

void CAIDestroyable::load(const json& j, TEntityParseContext& ctx) {
  setEntity(ctx.current_entity);

  Init();

  trigger_actor = j.value("trigger_actor", "");
  destroy_time = j.value("destroy_time", 1.0f);
  acum_time = 0;
}



void CAIDestroyable::IdleState()
{
  CEntity *c_trigger = (CEntity *)getEntityByName(trigger_actor);
  if (!c_trigger)
	  return;

  TCompTransform *mypos = getMyTransform();
  TCompTransform *ppos = c_trigger->get<TCompTransform>();

  float distance = VEC3::Distance(mypos->getPosition(), ppos->getPosition());
  if (distance < 0.5f) {
	  acum_time = 0.0f;
	  ChangeState("trigger_destroy");
  }
}


void CAIDestroyable::TriggerDestroyState(float dt)
{
	acum_time += dt;
	if (acum_time >= destroy_time)
		ChangeState("destroy");
}

void CAIDestroyable::DestoyState()
{
	CHandle(this).getOwner().destroy();
}
