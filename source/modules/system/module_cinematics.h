#pragma once

#include "modules/module.h"

class CModuleCinematics : public IModule {
public:

  CModuleCinematics(const std::string& name);
  bool start() override;
  bool stop() override;
  void update(float delta) override;
  void render() override;

  void activate(std::string name);

private:

  //Cinematicas guardadas por el propio nombre de la cinematica y el conjunto de camaras que las componen en orden de aparicion
  std::map<std::string, std::vector<CHandle>> cinematics;

  void load();

};
