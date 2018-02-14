#pragma once

#include "modules/module.h"
#include "entity/entity.h"

class IAIController;

class CModuleTimer : public IModule {

public:
	float time_slower;

	CModuleTimer(const std::string& aname) : IModule(aname) { }
	bool start() override;
  void update(float delta) override;
  void render() override;
	const float getTimeSlower();
	const void setTimeSlower(float time);
};

