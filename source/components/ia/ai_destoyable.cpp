#include "mcv_platform.h"
#include "handle/handle.h"
#include "ai_destoyable.h"
#include "entity/entity_parser.h"
#include "components/juan/comp_transform.h"
#include "components/sound/comp_sound.h"
#include "render/render_utils.h"

DECL_OBJ_MANAGER("ai_destroyable", CAIDestroyable);

using namespace physx;

void CAIDestroyable::debugInMenu() {
    IAIController::debugInMenu();
    ImGui::Text("acum_time %f", acum_time);
}

void CAIDestroyable::registerMsgs() {
    DECL_MSG(CAIDestroyable, TMsgDestroy, onTriggerEnter);
		DECL_MSG(CAIDestroyable, TMsgRegisterDestoyableSon, registerSon);
}

void CAIDestroyable::onTriggerEnter(const TMsgDestroy& msg) {
	if (!recovering) {
		acum_time = 0.0f;
		ChangeState("trigger_destroy");
	}
}

void CAIDestroyable::registerSon(const TMsgRegisterDestoyableSon & msg)
{
	sons.push_back(msg.name);
}

void CAIDestroyable::load(const json& j, TEntityParseContext& ctx) {
    setEntity(ctx.current_entity);

		sons.clear();

    destroy_time = j.value("destroy_time", 1.0f);
    recover_time = j.value("recover_time", 5.0f);
    acum_time = 0;
    Init();
}

void CAIDestroyable::Init()
{
    // insert all states in the map
    AddState("idle", (statehandler)&CAIDestroyable::IdleState);
    AddState("trigger_destroy", (statehandler)&CAIDestroyable::TriggerDestroyState);
    AddState("transition_destroy", (statehandler)&CAIDestroyable::TransitionDestroyState);
    AddState("destroy", (statehandler)&CAIDestroyable::DestroyState);

    // reset the state
    onStart = true;
    ChangeState("idle");

}

void CAIDestroyable::IdleState()
{
    if (onStart) {
        change_mesh(0);
        onStart = false;
    }
}


void CAIDestroyable::TriggerDestroyState(float dt)
{
    acum_time += DT;
    if (acum_time >= destroy_time)
    {
		recovering = true;
        acum_time = 0;
        //has_mesh = change_mesh(1);
				//CEntity* entity = CHandle(this).getOwner();
				for (int i = 0; i < sons.size(); i++) {
					CEntity* entity = (CEntity*)getEntityByName(sons[i]);
					TMsgActivateAnim msg;
					entity->sendMsg(msg);                    
				}        
        ChangeState("transition_destroy");
    }
}

void CAIDestroyable::TransitionDestroyState(float dt)
{

    TCompTransform *mypos = getMyTransform();
    if (!mypos) {
        return;
    }
    
    current_pos = mypos->getPosition();

    if (!has_mesh)
        mypos->setPosition(VEC3(0.f, 0.f, 0.f));
    TCompCollider* comp_collider = get<TCompCollider>();
    if (comp_collider)
    {
        comp_collider->config.group = EnginePhysics.getFilterByName("windstrike");
        comp_collider->config.mask = -4;
        PxRigidActor* rigidActor = ((PxRigidActor*)comp_collider->actor);
        PxTransform tr = rigidActor->getGlobalPose();
        tr.p = PxVec3(0, 0, 0);
        rigidActor->setGlobalPose(tr);
    }
    CEntity* e = h_entity;
    std::string name = e->getName();
    EngineSound.emitPositionalEvent("destroyable", name);
    ChangeState("destroy");   
}

void CAIDestroyable::DestroyState(float dt)
{       
    acum_time += DT;   
    if (acum_time >= recover_time)
    {
        acum_time = 0;        
        TCompTransform *mypos = getMyTransform();
        if (!mypos) {
            return;
        }
				for (int i = 0; i < sons.size(); i++) {
					CEntity* entity = (CEntity*)getEntityByName(sons[i]);
					TMsgDesactivateAnim msg;
					entity->sendMsg(msg);
				}
        bool has_mesh = change_mesh(0);



        mypos->setPosition(current_pos);
        TCompCollider* comp_collider = get<TCompCollider>();
        if (comp_collider)
        {
            comp_collider->config.group = EnginePhysics.getFilterByName("scenario");
            comp_collider->config.mask = EnginePhysics.getFilterByName("all");
            PxRigidActor* rigidActor = ((PxRigidActor*)comp_collider->actor);
            PxTransform tr = rigidActor->getGlobalPose();
            tr.p = PxVec3(current_pos.x, current_pos.y, current_pos.z);
            rigidActor->setGlobalPose(tr);
        }
				recovering = false;
        ChangeState("idle");
    }
}

void CAIDestroyable::change_color(VEC4 color) {
    TCompRender *my_render = getMyRender();
    if (my_render)
    {
        my_render->color = color;
    }
}