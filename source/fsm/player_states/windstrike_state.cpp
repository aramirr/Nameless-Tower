#include "mcv_platform.h"
#include "windstrike_state.h"
#include "fsm/context.h"
#include "entity/entity_parser.h"
#include "components/player/comp_player_controller.h"

namespace FSM
{/*
	void WindstrikeState::onStart(CContext& ctx) const
	{		
		CEntity* e = ctx.getOwner();
		TEntityParseContext ctx_w;
		ctx_w.entity_starting_the_parse = CHandle(this).getOwner();
		ctx_w.root_transform = *(TCompTransform*)e->get<TCompTransform>();;
		if (parseScene("data/prefabs/windstrike.prefab", ctx_w)) {
			assert(!ctx_w.entities_loaded.empty());
		}
	}

	bool WindstrikeState::load(const json& jData)
	{		
		return true;
	}

	bool WindstrikeState::update(float dt, CContext& ctx) const
	{		
		return false;
	}

	void WindstrikeState::onFinish(CContext& ctx) const {
		ctx.setVariable("windstrike", false);
	}
	*/
}

