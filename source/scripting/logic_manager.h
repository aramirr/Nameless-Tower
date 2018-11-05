#pragma once 
class LogicManager
{


public:

	LogicManager();
	void printDbg(const char* msg);
	
  // Hide - Unhide Meshes
  void appearEntity(const char* name);
  void disappearEntity(const char* name);
  void renderOnlyShadows(const char* name, bool left);
  void renderEverything(const char* name, bool left);

  // Cinematics
  void activateCinematic(const char* name, bool left);
  void deactivateCinematic(const char* name, bool left);
	void activateBandCinematics(bool left);
	void deactivateBandCinematics(bool left);

  // Misc
	void setAmbientAdjustment(float value);
  void setExposureAdjustment(float value, bool left);
	void playLastCinematic();
  void openDoor(const char* name);
  void closeDoor(const char* name);
	void activateAnim(const char* name, float wait_time);
	void setTemblor(bool temblor, bool left);
	void loadScene(std::string name);
	void unloadScene(std::string name);
	void setVignettingAdjustment(float value, bool left);
	void setFadeOutAdjustment(float value, bool left);
	void fundidoNegroFinal();

  // UI
	void activateText(const char* name);
	void disactivateText(const char* name);
	void activateCredits();
	void desactivateCinematics();

  // Lights
  void setDirLightIntensity(const char* name, float value, bool left);
  void setPointLightIntensity(const char* name, float value, bool left);
	void setNajaInterior(bool left);
	void setnajaExterior(bool left);
	void setRunnerInterior(bool left);
	void setRunnerExterior(bool left);

	// Particles
	void startEmiter(const char* name, bool left);
	void stopEmiter(const char* name, bool left);
	void addGrass(int id, bool left);
	void deleteGrass(int id, bool left);
	void updateGrassRender();

  // Change level
  void changeGameState(const char* name);

  // utils
  bool applyFunction(bool left);
	void playAmbientSound(bool left);
	void playAmbientNight();
  void playInteriorSound(bool left);
  void playPositionalSound(std::string name, std::string entityName);
  void playSound(bool left, std::string name);
	void stopSound(bool left, std::string name);
	void playDelayedSound(float time, std::string name);
  void activateTorch(std::string name);
  void scarePlayer();

	void activateCamaraBajada();
	void desactivateCamaraBajada();

  //Lua Cinematics
  void pausePlayer();
  void setAnim(int anim_id);
	void setAnimCycle(std::string name, int anim_id);
  void setCinematicBars();
  void regainControl(float time_to_wait);
  void killEntity(std::string name);
  void playCurve(std::string name);
  void set_ypr(std::string name, float y, float p, float r);

	void activateRunner();

	void setVolumen(float volumen, bool left);

};