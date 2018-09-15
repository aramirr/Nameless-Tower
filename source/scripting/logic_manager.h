#pragma once 
class LogicManager
{


public:

	LogicManager();
	void printDbg(const char* msg);
	
  // Hide - Unhide Meshes
  void appearEntity(const char* name);
	void disappearEntity(const char* name);
  void renderOnlyShadows(const char* name);
  void renderEverything(const char* name);

  // Cinematics
  void activateCinematic(const char* name);
  void deactivateCinematic(const char* name);

  // Misc
	void setAmbientAdjustment(float value);
  void setExposureAdjustment(float value);
	void playLastCinematic();
	void openDoor(const char* name);
	void activateAnim(const char* name);

  // UI
	void activateText(const char* name);
	void disactivateText(const char* name);

  //Lights
  void setDirLightIntensity(const char* name, float value);
  void setPointLightIntensity(const char* name, float value);

  // Change level
  void changeGameState(const char* name);

};