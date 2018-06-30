#pragma once

#include "modules/module.h"
#include "entity/entity.h"

class IAIController;

class CModuleTower : public IModule {

protected:
	float tower_radius = 31.5f;
	VEC3  last_checkpoint_position;
	float last_checkpoint_yaw;
	bool  last_checkpoint_looking_left;

public:
	CModuleTower(const std::string& aname) : IModule(aname) { }
	bool start() override;
  void update(float delta) override;
  void render() override;
	const VEC3 getLastCheckpoint();
	const void setLastCheckpoint(VEC3 checkpoint);
	const float getLastCheckpointYaw();
	const void setLastCheckpointYaw(float checkpoint_yaw);
	const bool getLastCheckpointLeft();
	const void setLastCheckpointLeft(bool checkpoint_left);
	const void disappearEntity(const std::string& name);
	const void appearEntity(const std::string& name);
	const void setAmbientAdjustment(float ambient);

  const void setDirLightIntensity(const std::string& name, float intensity);
  const void setPointLightIntensity(const std::string& name, float intensity);


	float getTowerRadius();
};

