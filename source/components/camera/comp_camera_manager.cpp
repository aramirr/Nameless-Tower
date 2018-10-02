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

void TCompCameraManager::activarTemblor()
{
	temblor = true;
}

void TCompCameraManager::desactivarTemblor()
{
	temblor = false;
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

	temblor = true;

  cameraActive = 0;
  currentTime = 0.f;
  totalTime = 0.f;
}

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
float dot(VEC2 a, VEC2 b) {
	return a.x * b.x + a.y * b.y;
}

float random(VEC2 st)
{
	return (float)(sin(dot(st,VEC2(12.9898, 78.233)))* 43758.5453123) % 1;
}

//float noise(VEC2 st)
//{
//	VEC2 i = floor(st);
//	VEC2 f = fract(st);
//	float a = random(i);
//	float b = random(i + VEC2(1.0, 0.0));
//	float c = random(i + VEC2(0.0, 1.0));
//	float d = random(i + VEC2(1.0, 1.0));
//	VEC2 u = f * f * (3.0 - 2.0 * f);
//	return mix(a, b, u.x) + (c - a)* u.y * (1.0 - u.x) + (d - b) * u.x * u.y;
//}
//
//#define octaves 3
//float fbm(VEC2 p)
//{
//	float value = 0.0;
//	float freq = 1.13;
//	float amp = 0.57;
//	for (int i = 0; i < octaves; i++)
//	{
//		value += amp * (noise((p - VEC2(1.0)) * freq));
//		freq *= 1.61;
//		amp *= 0.47;
//	}
//	return value;
//}
//
//float pat(VEC2 p)
//{
//	float time = iTime * 0.75;
//	VEC2 aPos = VEC2(sin(time * 0.035), sin(time * 0.05)) * 3.;
//	VEC2 aScale = VEC2(3.25);
//	float a = fbm(p * aScale + aPos);
//	VEC2 bPos = VEC2(sin(time * 0.09), sin(time * 0.11)) * 1.2;
//	VEC2 bScale = VEC2(0.75);
//	float b = fbm((p + a) * bScale + bPos);
//	VEC2 cPos = VEC2(-0.6, -0.5) + VEC2(sin(-time * 0.01), sin(time * 0.1)) * 1.9;
//	VEC2 cScale = VEC2(1.25);
//	float c = fbm((p + b) * cScale + cPos);
//	return c;
//}

VEC2 Shake(float maxshake, float mag, float dt)
{
	float speed = 20.0*mag;
	float shakescale = maxshake * mag;

	float time = dt * speed;			// speed of shake

	VEC2 p1 = VEC2(0.25, 0.25);
	VEC2 p2 = VEC2(0.75, 0.75);
	p1 += VEC2(time);
	p2 += VEC2(time);

	// random shake is just too violent...
	float val1 = random(p1);
	float val2 = random(p2);

	//float val1 = pat(p1);
	//float val2 = pat(p2);
	val1 = lerp(val1, 0.f, 1.f);
	val2 = lerp(val2, 0.f, 1.f);

	return VEC2(val1*shakescale, val2*shakescale);
}

//-----------------------------------------------------------------------
//
//-----------------------------------------------------------------------

void TCompCameraManager::update(float dt) {

	if (temblor && isPressed(VK_F3)) {
		CEntity* camera = (CEntity*)Engine.getCameras().getActiveCamera();
		TCompTransform* c = camera->get<TCompTransform>();
		float maxshake = 0.05;				// max shake amount
		float mag = 0.5 + sin(dt)*0.5;		// shake magnitude...

																				// *temp* , We will calc shakexy once in the vertex shader...
		VEC2 shakexy = Shake(maxshake, mag, dt);
		c->setPosition(c->getPosition() + shakexy);
		c->lookAt(c->getPosition() + shakexy, VEC3(0, c->getPosition().y,0) + shakexy);
	}
	else {
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
