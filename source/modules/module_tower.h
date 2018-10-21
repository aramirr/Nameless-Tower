#pragma once

#include "modules/module.h"
#include "entity/entity.h"


class IAIController;

class CModuleTower : public IModule {
	bool cargar = true;
	bool changeExposure;
	bool time_out = false;
	float current_time = 0.0f;
	float total_wait_time = 0.0f;
	float newExposure;
	float oldExposure;
	float defaultExposure;
	std::string current_cinematic;

	float bandsValue = 0.f;
	bool bandCinematics = false;

	// Activate Runner
	bool activate_runner = false;
	float timer_runner = 0.f;
	bool build_runner = false;
	bool changed_runner_mesh = false;
	bool runner_scream = false;
	bool runner_chase = false;
	bool start_anim = false;
	bool turn_player = false;



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
  const void renderOnlyShadows(const std::string& name);
  const void renderEverything(const std::string& name);
  const void setAmbientAdjustment(float ambient);
  const void setExposureAdjustment(float exposure);
	const void setBandsCinematics(bool _band);
  const void openDoor(const std::string& name);
  const void closeDoor(const std::string& name);
	const void activateAnim(const std::string& name, float wait_time);
  const void activateCinematic(const std::string& name);
  const void deactivateCinematic(const std::string& name);
  const void setDirLightIntensity(const std::string& name, float intensity);
  const void setPointLightIntensity(const std::string& name, float intensity);
	void setExposure(float _exposure);
	void wait_seconds(float num_seconds);
	void activateRunner();

	float getTowerRadius();

    float checkAngle(float alpha, VEC3 pos1);
};

