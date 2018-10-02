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

  // Misc
	void setAmbientAdjustment(float value);
  void setExposureAdjustment(float value, bool left);
	void playLastCinematic();
	void openDoor(const char* name);
	void activateAnim(const char* name);

  // UI
	void activateText(const char* name);
	void disactivateText(const char* name);

  // Lights
  void setDirLightIntensity(const char* name, float value, bool left);
  void setPointLightIntensity(const char* name, float value, bool left);
	void setNajaInterior(bool left);
	void setnajaExterior(bool left);

	// Particles
	void startEmiter(const char* name, bool left);
	void stopEmiter(const char* name, bool left);

  // Change level
  void changeGameState(const char* name);

  // utils
  bool applyFunction(bool left);
  void playAmbientSound(bool left);
  void playInteriorSound(bool left);
  void playSound(bool left, std::string name);
  void stopSound(bool left, std::string name);

  //Lua Cinematics
  void pausePlayer();
  void setAnim();
  void setCinematicBars();
  void regainControl(float time_to_wait);

};