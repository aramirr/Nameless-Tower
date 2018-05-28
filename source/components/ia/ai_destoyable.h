#ifndef _AIC_PATROL
#define _AIC_PATROL

#include "ai_controller.h"

class CAIDestroyable : public IAIController
{
    float destroy_time;
    float acum_time;
    float recover_time;
    VEC3 current_pos;
    bool has_mesh = false;
    bool onStart = true;

    void onTriggerEnter(const TMsgDestroy& msg);

    DECL_SIBLING_ACCESS();

public:
    void load(const json& j, TEntityParseContext& ctx);
    void debugInMenu();

    void IdleState();
    void TriggerDestroyState(float dt);
    void DestroyState(float dt);
    void TransitionDestroyState(float dt);
    void change_color(VEC4 color);

    void Init();
    static void registerMsgs();
};

#endif