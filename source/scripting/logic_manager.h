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

    void activateCinematic(const char* name);
    void deactivateCinematic(const char* name);

  // Misc
	void setAmbientAdjustment(float value);
	void playLastCinematic();
	void openDoor(const char* name);
	void activateAnim(const char* name);

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