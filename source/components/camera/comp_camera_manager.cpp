#include "mcv_platform.h"
#include "comp_camera_manager.h"
#include "components/juan/comp_transform.h"
#include "components/player/comp_player_controller.h"
#include "resources/json_resource.h"
#include "components/juan/comp_name.h"
#include "components/fsm/comp_fsm.h"

DECL_OBJ_MANAGER("cameraManager", TCompCameraManager);

static Interpolator::TSineInOutInterpolator interpolator;

bool TCompCameraManager::isGrounded()
{
	TCompPlayerController* pc = player->get<TCompPlayerController>();
	return pc->isGrounded();
}

bool TCompCameraManager::isForward()
{
	TCompPlayerController* pc = player->get<TCompPlayerController>();
	return !pc->isForward();
}

void TCompCameraManager::attachPlayer(const TMsgAttachTo & msg) {
	platform = (CEntity *)msg.h_attached;
	TCompTransform* pl = platform->get<TCompTransform>();
	assert(pl);
	VEC3 plPos = pl->getPosition();

	TCompTransform* p = player->get<TCompTransform>();
	assert(p);
	VEC3 pPos = p->getPosition();

	if (pPos.y > plPos.y)inPlatform = true;
}

void TCompCameraManager::detachPlayer(const TMsgDetachOf & msg) {
	if (inPlatform) {
		inPlatform = false;
		jumpinPlatform = true;
	}
}

void TCompCameraManager::loadCinematics()
{
	//Cargamos el Json con todas las cinematicas en la estructura
	const json& j = Resources.get("data/cinematics.json")->as<CJsonResource>()->getJson(); //loadJson("data/cinematics.json");
	assert(j.is_array());

	for (int i = 0; i < j.size(); ++i) {

		auto& j_item = j[i];
		assert(j_item.is_object());

		if (j_item.count("cinematic")) {
			auto& j_cinematic = j_item["cinematic"];

			std::string cinematic_name = j_cinematic["name"];
			dbg(cinematic_name.c_str());

			std::vector<std::pair<Camera, float>> cameras;

			auto& j_cameras = j_cinematic["cameras"];
			for (auto it = j_cameras.begin(); it != j_cameras.end(); ++it) {
				VEC3 pos;
				VEC3 lookAt;
				float time;

				std::string str = (it.value()).get<std::string>();

				//dbg("------------------------------------------------------------------------------\n");
				//dbg(str.c_str());
				//dbg("\n");

				int n = sscanf(str.c_str(), "%f %f %f %f %f %f %f", &pos.x, &pos.y, &pos.z, &lookAt.x, &lookAt.y, &lookAt.z, &time);
				assert(n == 7);

				if (time <= 0.f)time = 0.1f;

				Camera cam;
				cam.camPos = pos;
				cam.camLookAt = lookAt;

				std::pair<Camera, float> p;
				p.first = cam;
				p.second = time;

				cameras.push_back(p);
			}

			cinematics.insert(std::make_pair(cinematic_name, cameras));
		}

	}
}

void TCompCameraManager::registerMsgs() {
	DECL_MSG(TCompCameraManager, TMsgAttachTo, attachPlayer);
	DECL_MSG(TCompCameraManager, TMsgDetachOf, detachPlayer);
}

void TCompCameraManager::activateCinematic(std::string name)
{
	if (!godMode & cinematics.find(name) != cinematics.end()) {
		cinematicName = name;

    CEntity* player = (CEntity*)getEntityByName("The Player");

    TMsgSetFSMVariable pauseMsg;
    pauseMsg.variant.setName("pause");
    pauseMsg.variant.setBool(true);

    player->sendMsg(pauseMsg);

		onCinematics = true;
	}
}

void TCompCameraManager::debugInMenu() {

	/*float fov_deg = rad2deg(getFov());
	float new_znear = getZNear();
	float new_zfar = getZFar();*/
	//float fov_deg = 0.f;
	//ImGui::DragFloat("Fov", &fov_deg, 0.1f, 30.f, 175.f);
}

void TCompCameraManager::load(const json& j, TEntityParseContext& ctx) {

	// ..
	player = (CEntity *)getEntityByName("The Player");

	//float fov_deg = j.value("fov", rad2deg(getFov()));
	//float z_near = j.value("z_near", getZNear());
	//float z_far = j.value("z_far", getZFar());
	//setPerspective(deg2rad(fov_deg), z_near, z_far);

	pForwarding = true;

	carga = true;

	inPlatform = false;
	jumpinPlatform = false;
	exitPlatform = false;

	lateral = false;

	onCinematics = false;
	cinemating = false;

	godMode = false;

	cinematics.clear();

	cameraActive = 0;
	currentTime = 0.f;
	totalTime = 0.f;
}

void TCompCameraManager::update(float dt) {

  if (isPressed(VK_F1)) {
    CEntity* player = (CEntity*)getEntityByName("The Player");

    TMsgSetFSMVariable pauseMsg;
    pauseMsg.variant.setName("pause");
    pauseMsg.variant.setBool(true);

    player->sendMsg(pauseMsg);
   
    godMode = true;
  }
  if (isPressed(VK_F2)) {
    CEntity* player = (CEntity*)getEntityByName("The Player");
    
    TMsgSetFSMVariable pauseMsg;
    pauseMsg.variant.setName("pause");
    pauseMsg.variant.setBool(false);

    player->sendMsg(pauseMsg);

    godMode = false;
  }


	if (carga) {																				// CONFIGURACION INICIAL DEL MANAGER DE CAMARAS
		CHandle h_camera = getEntityByName("camera_orbit_IZQ");
		Engine.getCameras().setDefaultCamera(h_camera);

		h_camera = getEntityByName("the_camera");
		Engine.getCameras().setOutputCamera(h_camera);

		lateral = true;

		carga = false;

		loadCinematics();
	}

	if (onCinematics) {																			// REALIZAMOS UNA CINEMATICA

		CEntity* cam = (CEntity*)getEntityByName("the_camera");

		TCompTransform* ct = cam->get<TCompTransform>();
		assert(ct);

		if (!cinemating) {

			cameras.clear();
			cameras = cinematics[cinematicName];

			ct->setPosition(cameras[0].first.camPos);
			ct->lookAt(cameras[0].first.camPos, cameras[0].first.camLookAt);

			Engine.getCameras().blendInCamera(cam, 0.1f, CModuleCameras::EPriority::GAMEPLAY, &interpolator);

			cameraActive = 1;

			cinemating = true;
		}
		else {
			currentTime += dt;

			//TO DO
			//Ir interpolando manualmente entre las camaras que componen la cinematica en cuestion
			//...

			float ratio = ((currentTime - totalTime) / cameras[cameraActive].second);

			VEC3 startP = cameras[cameraActive - 1].first.camPos;
			VEC3 startL = cameras[cameraActive - 1].first.camLookAt;
			VEC3 endP = cameras[cameraActive].first.camPos;
			VEC3 endL = cameras[cameraActive].first.camLookAt;
			VEC3 camP;
			VEC3 camL;

			if (ratio <= 0.0f) {
				camP = startP;
				camL = startL;
			}
			else if (ratio >= 1.0f) {
				camP = endP;
				camL = endL;

				totalTime += cameras[cameraActive].second;

				cameraActive++;
			}
			else {
				camP = -(endP - startP) * 0.5f * (cosf((float)(M_PI * ratio)) - 1.f) + startP;
				camL = -(endL - startL) * 0.5f * (cosf((float)(M_PI * ratio)) - 1.f) + startL;
			}

			ct->setPosition(camP);
			ct->lookAt(camP, camL);

			/*	std::string str = std::to_string(currentTime);
			std::string str2 = std::to_string(ratio);
			std::string str3 = std::to_string(cameraActive);
			std::string str4 = std::to_string(cameras.size());

			dbg("------------------------------------------------------------------------------\n");
			dbg(str.c_str());
			dbg("\n");
			dbg(str2.c_str());
			dbg("\n");
			dbg(str3.c_str());
			dbg("\n");
			dbg(str4.c_str());
			dbg("\n");*/

			if (cameraActive == cameras.size()) {
				cinemating = false;
				onCinematics = false;
				currentTime = 0.f;
				totalTime = 0.f;

        CEntity* player = (CEntity*)getEntityByName("The Player");

        TMsgSetFSMVariable pauseMsg;
        pauseMsg.variant.setName("pause");
        pauseMsg.variant.setBool(false);

        player->sendMsg(pauseMsg);

			}
		}
	}
	else {																						// MANAGER DE CAMARAS POR DEFECTO
		if (godMode) {
			CHandle h_camera = getEntityByName("camera_god");
			Engine.getCameras().blendInCamera(h_camera, 1.f, CModuleCameras::EPriority::GAMEPLAY, &interpolator);
		}
		else {
			TCompTransform* p = player->get<TCompTransform>();
			assert(p);
			VEC3 pPos = p->getPosition();

			static Interpolator::TSineInOutInterpolator interpolator;

			if (exitPlatform && isGrounded())exitPlatform = false;

			if (jumpinPlatform) {
				jumpinPlatform = false;
				exitPlatform = true;
			}

			if ((inPlatform || exitPlatform) && lateral) {
				CHandle h_camera = getEntityByName("camera_platform");
				Engine.getCameras().blendInCamera(h_camera, 1.f, CModuleCameras::EPriority::GAMEPLAY, &interpolator);

				lateral = false;
			}
			else if (!inPlatform && !exitPlatform) {
				bool playerForward = isForward();  //Vemos si el player se esta moviendo hacia delante o hacia atras

				CEntity* camIzq = (CEntity *)getEntityByName("camera_orbit_IZQ");
				TCompTransform* ci = camIzq->get<TCompTransform>();
				assert(ci);
				VEC3 cip = ci->getPosition();
				float distanceCamIzq = VEC3::Distance(pPos, cip);

				CEntity* camDer = (CEntity *)getEntityByName("camera_orbit_DER");
				TCompTransform* cd = camDer->get<TCompTransform>();
				assert(cd);
				VEC3 cdp = cd->getPosition();
				float distanceCamDer = VEC3::Distance(pPos, cdp);

				if ((playerForward && distanceCamDer > 9.f)) {

					CHandle h_camera = getEntityByName("camera_orbit_IZQ");
					Engine.getCameras().blendInCamera(h_camera, 1.f, CModuleCameras::EPriority::GAMEPLAY, &interpolator);

					pForwarding = true;

					lateral = true;

				}
				else if (distanceCamIzq > 9.f) {
					CHandle h_camera = getEntityByName("camera_orbit_DER");
					Engine.getCameras().blendInCamera(h_camera, 1.f, CModuleCameras::EPriority::GAMEPLAY, &interpolator);

					pForwarding = false;

					lateral = true;

				}
			}
		}

	}
}
