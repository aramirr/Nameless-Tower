#pragma once

#include "modules/module.h"

class CModuleCinematics : public IModule {
public:

  CModuleCinematics(const std::string& name);
  bool start() override;
  bool stop() override;

  void activate(std::string name);

  void loadCinematics();

private:

	struct Camera {
		VEC3 camPos;
		VEC3 camLookAt;
	};

  //Cinematicas guardadas por el propio nombre de la cinematica y el conjunto de camaras que las componen en orden de aparicion
  std::map<std::string, std::vector<std::pair<Camera, float>>> cinematics;

};
