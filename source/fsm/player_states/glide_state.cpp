#include "mcv_platform.h"
#include "glide_state.h"
#include "fsm/context.h"
#include "components/player/comp_player_controller.h"

namespace FSM
{
	void GlideState::onStart(CContext& ctx) const
	{
		CEntity* e = ctx.getOwner();
		TCompPlayerController* player = e->get<TCompPlayerController>();
		TCompTransform *c_my_transform = e->get<TCompTransform>();
		ctx.setVariable("is_grounded", false);
		player->is_grounded = false;
	}

	bool GlideState::load(const json& jData)
	{
		return true;
	}

	bool GlideState::update(float dt, CContext& ctx) const
	{		
		return false;
	}

	void GlideState::onFinish(CContext& ctx) const {
		ctx.setVariable("glide", false);
	}

}

