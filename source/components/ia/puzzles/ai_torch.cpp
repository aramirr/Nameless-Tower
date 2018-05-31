#include "mcv_platform.h"
#include "handle/handle.h"
#include "ai_torch.h"
#include "entity/entity_parser.h"
#include "components/juan/comp_transform.h"
#include "render/render_utils.h"
#include "modules/system/module_physics.h"
#include "components/player/comp_player_controller.h"
#include "render/render_objects.h" 



DECL_OBJ_MANAGER("ai_torch", CAITorch);

void CAITorch::Init()
{

	AddState("active", (statehandler)&CAITorch::ActiveState);
	AddState("inactive", (statehandler)&CAITorch::InactiveState);
	ChangeState("active");
}

void CAITorch::debugInMenu() {

	IAIController::debugInMenu();
    ImGui::DragFloat("Intensity", &intensity, 0.01f, 0.f, 100.f);
    ImGui::ColorEdit3("Color", &color.x);
    ImGui::DragFloat("Radius", &radius, 0.01f, 0.f, 100.f);
}

MAT44 CAITorch::getWorld() {
    TCompTransform* c = get<TCompTransform>();
    if (!c)
        return MAT44::Identity;
    return MAT44::CreateScale(radius) * c->asMatrixCustomPosition(fire_position);
}

// -------------------------------------------------
void CAITorch::renderDebug() {
    // Render a wire sphere
    auto mesh = Resources.get("data/meshes/UnitSphere.mesh")->as<CRenderMesh>();
    renderMesh(mesh, getWorld(), VEC4(1, 1, 1, 1));
}

void CAITorch::load(const json& j, TEntityParseContext& ctx) {
    setEntity(ctx.current_entity);
	timer_limit = j.value("time_limit", 5.0f);
	puzzle_name = j.value("puzzle", "");
	if (puzzle_name != "") {
		in_puzzle = true;
		CEntity* puzzle_entity = (CEntity*)getEntityByName(puzzle_name);
        TMsgAttachTo activate_msg;
        CEntity* e = h_entity;
        activate_msg.h_attacher = h_entity;
        puzzle_entity->sendMsg(activate_msg);
	}

    if (j.count("color"))
        color = loadVEC4(j["color"]);
    intensity = j.value("intensity", intensity);
    radius = j.value("radius", radius);
    initial_radius = radius;

    Init();
}

void CAITorch::registerMsgs() {
DECL_MSG(CAITorch, TMsgDeactivateTorch, deactivate);
}


void CAITorch::ActiveState(float dt)
{	
    TCompTransform* my_transform = getMyTransform();
    if (on_start) {
        fire_position = my_transform->getPosition();
        fire_position.y += 0.8f;
        on_start = false;
    }
    if (b_fuego) {
        EngineParticles.addFuegoTest(fire_position);
        b_fuego = false;
    }
    
}


void CAITorch::InactiveState(float dt)
{	
	if (!in_puzzle) {
		timer += dt;
		if (timer > timer_limit) {
			activate();
		}
	}	
}

void CAITorch::activate() {
	active = true;
	TCompRender *my_render = getMyRender();
	my_render->self_illumination = 1;
    
	ChangeState("active");
}


void CAITorch::deactivate(const TMsgDeactivateTorch& msg) {
	if (active) {
		active = false;
		TCompRender *my_render = getMyRender();
		my_render->self_illumination = 1;
		timer = 0;
		ChangeState("inactive");
		if (in_puzzle) {
			TMsgActivateTorchPuzzle activate_msg;
			CEntity* e_collider_entity = (CEntity*)getEntityByName(puzzle_name);
            CEntity* e = h_entity;
			e_collider_entity->sendMsg(activate_msg);
            attached = true;
		}
	}	
}

void CAITorch::simulateLight() {
    float r = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX))*DT;
    TCompTransform* my_transform = getMyTransform();

    int aux1 = rand() % 2;
    if (aux1 == 0) {
        color.x += r;
    }
    else {
        color.x -= r;
    }
    cb_light.light_color = color;

    cb_light.light_pos = fire_position;
    if (b_increasing_radius) {
        radius += r;
        intensity += r;
        int aux = rand() % 2;
        if (aux == 0 || radius > initial_radius + 0.2f)
            b_increasing_radius = false;
    }
    else {
        radius -= r;
        if (intensity -r > 0)
            intensity -= r;
        int aux = rand() % 2;
        if (aux == 0 || radius < initial_radius - 0.2f)
            b_increasing_radius = true;
    }
    cb_light.light_intensity = intensity;
    cb_light.light_radius = radius * my_transform->getScale();
    cb_light.light_view_proj_offset = MAT44::Identity;
    cb_light.updateGPU();
}