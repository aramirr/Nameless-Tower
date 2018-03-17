#include "mcv_platform.h"
#include "handle/handle.h"
#include "ai_spikes.h"
#include "entity/entity_parser.h"
#include "components/juan/comp_transform.h"
#include "render/render_utils.h"

DECL_OBJ_MANAGER("ai_spikes", CAISpikes);

void CAISpikes::Init()
{
  // insert all states in the map
  AddState("off", (statehandler)&CAISpikes::SpikesOffState);
  AddState("countdown", (statehandler)&CAISpikes::TriggerSpikeState);
  AddState("on", (statehandler)&CAISpikes::SpikesOnState);

  // reset the state
  if (fixed_spikes) {
	  ChangeState("on");
  }
  else {
	  ChangeState("off");
  }
}

void CAISpikes::debugInMenu() {

  IAIController::debugInMenu();
}

void CAISpikes::load(const json& j, TEntityParseContext& ctx) {
  setEntity(ctx.current_entity);

  fixed_spikes = j.value("fixed_spikes", true);
  time_to_trigger = j.value("time_to_trigger", 1.0f);
  time_to_return_to_off = j.value("time_to_return_to_off", 1.0f);
  acum_time = 0;

  Init();
}



void CAISpikes::SpikesOffState()
{
  TCompRender *my_render = getMyRender();
  my_render->is_active = false;
  change_color(VEC4(0, 1, 0, 1));
}


void CAISpikes::TriggerSpikeState(float dt)
{
	TCompRender *my_render = getMyRender();
	my_render->is_active = true;
	change_color(VEC4 (1, 0, 0, 1));
	acum_time += DT;
	if (acum_time >= time_to_trigger)
	{
		ChangeState("on");
		acum_time = 0;
	}
}

void CAISpikes::SpikesOnState(float dt)
{
	TCompRender *my_render = getMyRender();
	my_render->is_active = true;
	change_color(VEC4(0, 0, 1, 1));
	acum_time += DT;
	CEntity *player = (CEntity *)getEntityByName("The Player");
	assert(player);
	TCompTransform *p_trans = player->get<TCompTransform>();
	
	CEntity *current_e = h_entity;
	TCompTransform *curr_trans = current_e->get<TCompTransform>();
	float distance = VEC3::Distance(p_trans->getPosition(), curr_trans->getPosition());
	if (distance < 1) {
		TMsgKillPlayer kill_player_message;
		player->sendMsg(kill_player_message);
	}
	
	if (!fixed_spikes) {
		if (acum_time >= time_to_trigger)
		{
			ChangeState("off");
			acum_time = 0;
		}
	}
}

void CAISpikes::change_color(VEC4 color) {
	TCompRender *my_render = getMyRender();
	if (my_render)
	{
		my_render->color = color;
	}
}

void CAISpikes::registerMsgs() {
	DECL_MSG(CAISpikes, TMsgTriggerSpike, doOnCollision);
}

void CAISpikes::doOnCollision(const TMsgTriggerSpike& msg)
{
	if (state == "off") {
		ChangeState("countdown");
	}
	else if(state == "on") 
	{
		TMsgKillPlayer kill_player_message;
		CEntity *e = msg.h_player;
		e->sendMsg(kill_player_message);
	}
}