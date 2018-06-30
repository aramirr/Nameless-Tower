#pragma once 
class LogicManager
{


public:

	LogicManager();
	void printDbg(const char* msg);
	
  // Hide - Unhide Mesh
  void appearEntity(const char* name);
	void disappearEntity(const char* name);


	void activateCamera(const char* name);
	void disactivateCamera(const char* name);

  // Misc
	void setAmbientAdjustment(float value);
	void playLastCinematic();

  // UI
	void activateText(const char* name);
	void disactivateText(const char* name);

  //Lights
  void setDirLightIntensity(const char* name, float value);
  void setPointLightIntensity(const char* name, float value);

};


class LogicPlayer
{


public:

	LogicPlayer();
	VEC3 getPlayerPosition();
	void setPlayerPosition(float x, float y, float z);

};