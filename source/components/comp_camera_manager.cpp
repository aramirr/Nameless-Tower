#include "mcv_platform.h"
#include "comp_camera_manager.h"
#include "comp_transform.h"
#include "comp_player_controller.h"

DECL_OBJ_MANAGER("cameraManager", TCompCameraManager);

bool TCompCameraManager::isForward()
{

  TCompPlayerController* pc = player->get<TCompPlayerController>();
  return !pc->isForward();
	//if (player.x > 0 && player.z < 0) {								// CUADRANTE 1 (+-)
	//	if (frontPlayer.x < 0 && frontPlayer.z < 0) return true;
	//	return false;
	//}
	//else if (player.x < 0 && player.z < 0) {						// CUADRANTE 2 (--)
	//	if (frontPlayer.x < 0 && frontPlayer.z > 0) return true;
	//	return false;
	//}
	//else if (player.x < 0 && player.z > 0) {						// CUADRANTE 3 (-+)
	//	if (frontPlayer.x > 0 && frontPlayer.z > 0) return true;
	//	return false;
	//}
	//else if (player.x > 0 && player.z > 0) {						// CUADRANTE 4 (++)
	//	if (frontPlayer.x > 0 && frontPlayer.z < 0) return true;
	//	return false;
	//}
	//else return false;
}

void TCompCameraManager::debugInMenu() {

	/*float fov_deg = rad2deg(getFov());
	float new_znear = getZNear();
	float new_zfar = getZFar();*/
	float fov_deg = 0.f;
	ImGui::DragFloat("Fov", &fov_deg, 0.1f, 30.f, 175.f);
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
}

void TCompCameraManager::update(float dt) {

	if (carga) {
		CHandle h_camera = getEntityByName("camera_orbit_IZQ");
		Engine.getCameras().setDefaultCamera(h_camera);

		h_camera = getEntityByName("the_camera");
		Engine.getCameras().setOutputCamera(h_camera);

		carga = false;
	}

	TCompTransform* p = player->get<TCompTransform>();
	assert(p);
	VEC3 pPos = p->getPosition();

	bool playerForward = isForward();  //Vemos si el player se esta moviendo hacia delante o hacia atras

	static Interpolator::TSineInOutInterpolator interpolator;

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

	if (playerForward && distanceCamDer > 9.f) {
		//if (!pForwarding) {
			CHandle h_camera = getEntityByName("camera_orbit_IZQ");
      Engine.getCameras().blendInCamera(h_camera, 1.f, CModuleCameras::EPriority::GAMEPLAY , &interpolator);

			pForwarding = true;
		//}
	}
	else if (distanceCamIzq > 9.f){
		//if (pForwarding) {
			CHandle h_camera = getEntityByName("camera_orbit_DER");
			Engine.getCameras().blendInCamera(h_camera, 1.f, CModuleCameras::EPriority::GAMEPLAY, &interpolator);

			pForwarding = false;
		//}
	}

}

