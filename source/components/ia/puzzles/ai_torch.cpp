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
    y_offset = j.value("y_offset", 0.f);
    smoke_y_offset = j.value("smoke_y_offset", 0.f);
    z_offset = j.value("z_offset", 0.f);
    x_offset = j.value("x_offset", 0.f);
	puzzle_name = j.value("puzzle", "");
	if (puzzle_name != "") {
		in_puzzle = true;
		CEntity* puzzle_entity = (CEntity*)getEntityByName(puzzle_name);
        TMsgAttachTo activate_msg;
        CEntity* e = h_entity;
        activate_msg.h_attacher = h_entity;
        puzzle_entity->sendMsg(activate_msg);
	}

	if (j.count("color")) {
		color = loadVEC4(j["color"]);
		initial_color = color;
	}
    intensity = j.value("intensity", intensity);
	radius = j.value("radius", radius);
    scale = j.value("scale", scale);
    thin = j.value("thin", thin);
    violeta = j.value("violeta", false);
    azul = j.value("azul", false);
    initial_radius = radius;
    render = j.value("render", true);
    frames = j.value("frames", 0.5f);
    smoke_frames = j.value("smoke_frames", 0.55f);
    Init();
}

void CAITorch::registerMsgs() {
    DECL_MSG(CAITorch, TMsgDeactivateTorch, deactivate);
    DECL_MSG(CAITorch, TMsgActivateTorch, activateMsg);
}

void CAITorch::ActiveState(float dt)
{	
    if (render) {
        TCompTransform* my_transform = getMyTransform();
        if (on_start) {
            fire_position = my_transform->getPosition();
            fire_position.y += y_offset;
            fire_position.z += z_offset;
            fire_position.x += x_offset;
            on_start = false;
        }
        if (b_fuego) {
            if (violeta) {
                id = EngineBillboards.addFuegoVioleta(fire_position, scale, thin);
            }
            else if (azul){
                id = EngineBillboards.addFuegoAzul(fire_position, scale, smoke_y_offset);
            } else
                id = EngineBillboards.addFuegoTest(fire_position, scale, thin);
            b_fuego = false;
        }
    }  
    
}

void CAITorch::InactiveState(float dt)
{	
	if (in_puzzle) {
		timer += DT;
        current_frames += DT;
		if (!apagando && current_frames > frames) {
			EngineBillboards.apagarFuegoAzul(id, scale);
            apagando = true;
            current_frames = 0;
		}
        if (apagando && !apagado && current_frames > smoke_frames) {
            EngineBillboards.prendiendoHumo(id, scale);
            apagado = true;
            current_frames = 0;
        }
	}	
}

void CAITorch::activate() {
	active = true;
    render = true;    
    apagando = false;
	TCompTransform* my_transform = getMyTransform();
    if (violeta)
        EngineBillboards.encenderFuegoVioleta(id, scale, thin);
    else if (azul) {
        EngineBillboards.encenderFuegoAzul(id, scale);
    }
    else
        EngineBillboards.encenderFuego(id, scale, thin);
	ChangeState("active");
}

void CAITorch::deactivate(const TMsgDeactivateTorch& msg) {
	if (active) {
        active = false;
        apagando = false;
        apagado = false;
		TCompTransform* my_transform = getMyTransform();
        EngineBillboards.apagandoFuegoAzul(id, scale);
		timer = 0;
        current_frames = 0;
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

void CAITorch::activateMsg(const TMsgActivateTorch& msg) {
    if (!active || !render) {
        activate();
    }
}

void CAITorch::simulateLight() {
	if (active) {
		float r = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX))*DT;
		if (r > 0.1f) r = 0.1f;

		TCompTransform* my_transform = getMyTransform();

		int aux1 = rand() % 2;
		if (aux1 == 0 && color.x < initial_color.x + 0.2f) {
			color.x += r;
		}
		else if (aux1 == 1 && color.x > initial_color.x - 0.2f){
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
			if (intensity - r > 0)
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
	else {
		cb_light.light_intensity = 0.f;
		cb_light.updateGPU();
	}
}