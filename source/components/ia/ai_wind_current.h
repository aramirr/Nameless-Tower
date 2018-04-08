#ifndef _AIC_FAN
#define _AIC_FAN

#include "ai_controller.h"

class CAIFan : public IAIController
{
	float force;
	CHandle	 attached;

	DECL_SIBLING_ACCESS();

public:
	void load(const json& j, TEntityParseContext& ctx);
	void debugInMenu();

	void Init();
	void IdleState(float dt);

	static void registerMsgs();
	void attachPlayer(const TMsgAttachTo& msg);
	void detachPlayer(const TMsgDetachOf& msg);
};

#endif