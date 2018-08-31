#include "mcv_platform.h"
#include "comp_camera_manager.h"
#include "components/juan/comp_transform.h"
#include "components/player/comp_player_controller.h"
#include "resources/json_resource.h"
#include "components/juan/comp_name.h"
#include "components/fsm/comp_fsm.h"

DECL_OBJ_MANAGER("cameraManager", TCompCameraManager);

static Interpolator::TSineInOutInterpolator interpolator;

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
      //dbg(cinematic_name.c_str());

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
  //...
}

void TCompCameraManager::load(const json& j, TEntityParseContext& ctx) {

  player = (CEntity *)getEntityByName("The Player");

  carga = true;

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
    pauseMsg.variant.setName("idle");
    pauseMsg.variant.setBool(true);

    player->sendMsg(pauseMsg);

    godMode = false;
  }


  if (carga) {																				// CONFIGURACION INICIAL DEL MANAGER DE CAMARAS
    CHandle h_camera = getEntityByName("camera_orbit_IZQ");
    Engine.getCameras().setDefaultCamera(h_camera);

    Engine.getCameras().blendInCamera(h_camera, 2.f, CModuleCameras::EPriority::GAMEPLAY, &interpolator);

    //h_camera = getEntityByName("the_camera");
    Engine.getCameras().setOutputCamera(h_camera);

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

        EngineUI.activeMainMenu();
        EngineUI.activateWidget("pantallaInicio");

        CEntity* player = (CEntity*)getEntityByName("The Player");

        TMsgSetFSMVariable pauseMsg;
        pauseMsg.variant.setName("pause");
        pauseMsg.variant.setBool(true);

        player->sendMsg(pauseMsg);

      }
    }
  }
  else {																						// MANAGER DE CAMARAS POR DEFECTO
    if (godMode) {
      CHandle h_camera = getEntityByName("camera_god");
      Engine.getCameras().blendInCamera(h_camera, 2.f, CModuleCameras::EPriority::GAMEPLAY, &interpolator);
    }
    else{  
      //CHandle h_camera = getEntityByName("camera_orbit_IZQ");
      //Engine.getCameras().blendInCamera(h_camera, 2.f, CModuleCameras::EPriority::GAMEPLAY, &interpolator);
    }
  }
}


void TCompCameraManager::activateCamera(const TMsgActiveCamera &msg) {
	CHandle h_camera = getEntityByName(msg.camera_name);
	Engine.getCameras().blendInCamera(h_camera, msg.blend_time, CModuleCameras::EPriority::GAMEPLAY, &interpolator);
}

void TCompCameraManager::deactivateCamera(const TMsgRemoveCamera &msg) {
    CHandle h_camera = getEntityByName(msg.camera_name);
    Engine.getCameras().blendOutCamera(h_camera, msg.blend_time);
}

void TCompCameraManager::registerMsgs() {
    DECL_MSG(TCompCameraManager, TMsgActiveCamera, activateCamera);
    DECL_MSG(TCompCameraManager, TMsgRemoveCamera, deactivateCamera);
}
