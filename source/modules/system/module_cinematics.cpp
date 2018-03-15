#include "mcv_platform.h"
#include "module_cinematics.h"
#include "components/comp_camera.h"
#include "resources/json_resource.h"
#include "components/comp_transform.h"
#include "components/comp_name.h"

CModuleCinematics::CModuleCinematics(const std::string & name) : IModule(name) {
	cinematics.clear();
}

bool CModuleCinematics::start() {
	return true;
}

bool CModuleCinematics::stop() {
	cinematics.clear();
	return true;
}

void CModuleCinematics::activate(std::string name) {

	//std::map<std::string, std::vector<std::pair<CHandle, float>>>::iterator it = cinematics.find(name);
	//assert(it == cinematics.end());

	CEntity* camManager = (CEntity *)getEntityByName("camera_manager");

	TMsgCinematicON msgON;
	camManager->sendMsg(msgON);

	//std::vector<std::pair<CHandle, float>> cameras = it->second;
	std::vector<std::pair<Camera, float>> cameras = cinematics[name];

	static Interpolator::TSineInOutInterpolator interpolator;

	dbg("------------------------------------------------------------------------------\n");
	dbg("%f", cameras.size());
	dbg("\n");

	for (int i = 0; i < cameras.size(); i++) {

		CEntity* cam = (CEntity*)getEntityByName("the_camera");
		
		TCompName* cn = cam->get<TCompName>();
		assert(cn);
		cn->setName((name + "_" + std::to_string(i)).c_str());

		TCompTransform* ct = cam->get<TCompTransform>();
		assert(ct);

		ct->setPosition(cameras[i].first.camPos);
		ct->lookAt(cameras[i].first.camPos, cameras[i].first.camPos + cameras[i].first.camLookAt);

		Engine.getCameras().blendInCamera(cam, cameras[i].second, CModuleCameras::EPriority::GAMEPLAY, &interpolator);

		Sleep(cameras[i].second * 100);

	}

	TMsgCinematicOFF msgOFF;
	camManager->sendMsg(msgOFF);
}

void CModuleCinematics::loadCinematics() {
	cinematics.clear();

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

				dbg("------------------------------------------------------------------------------\n");
				dbg(str.c_str());
				dbg("\n");

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

	//EJEMPLO DE CARGA JSON

	//auto& j_waypoints = j["waypoints"];
	//for (auto it = j_waypoints.begin(); it != j_waypoints.end(); ++it) {
	//	VEC3 p = loadVEC3(it.value());
	//	addWaypoint(p);
	//}

	//VEC3 v;
	//std::string str = j.get< std::string >();
	//int n = sscanf(str.c_str(), "%f %f %f", &v.x, &v.y, &v.z);
	//assert(n == 3);
	//return v;

}
