#pragma once

#include "components/comp_base.h"
#include "camera/camera.h"

class TCompCameraManager : public TCompBase {

	CEntity *player;
	CEntity *platform;

	bool carga;

	bool normal;

	bool godMode;

	/*
		CINEMATICAS
	*/

	bool onCinematics;
	bool cinemating;

	std::string cinematicName;

	int cameraActive;
	float currentTime;

	struct Camera {
		VEC3 camPos;
		VEC3 camLookAt;
	};

	void loadCinematics();

	float totalTime;

	//Cinematicas guardadas por el propio nombre de la cinematica y el conjunto de camaras que las componen en orden de aparicion
	std::map<std::string, std::vector<std::pair<Camera, float>>> cinematics;

	//Vector con las posiciones de las camaras que forman parte de una cinematica
	std::vector<std::pair<Camera, float>> cameras;

public:
	static void registerMsgs();

	void activateCinematic(std::string name);

	void debugInMenu();
	void load(const json& j, TEntityParseContext& ctx);
	void update(float dt);
	DECL_SIBLING_ACCESS();
};