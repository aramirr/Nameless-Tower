#include "mcv_platform.h"
#include "module_cinematics.h"
#include "components/comp_camera.h"
#include "resources/json_resource.h"

//TODO

CModuleCinematics::CModuleCinematics(const std::string & name) : IModule(name) {
	cinematics.clear();
}

bool CModuleCinematics::start() {
	
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

			std::vector<CHandle> cameras = std::vector<CHandle>();

			auto& j_cameras = j_cinematic["cameras"];
			for (auto it = j_cameras.begin(); it != j_cameras.end(); ++it) {
				VEC3 pos;
				VEC3 lookAt;
				
				std::string str = (it.value()).get<std::string>();

				dbg("------------------------------------------------------------------------------\n");
				dbg(str.c_str());
				dbg("\n");
			}
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

	return true;
}

bool CModuleCinematics::stop() {
	cinematics.clear();
	return true;
}

void CModuleCinematics::activate(std::string name) {

}

void CModuleCinematics::load() {

}
