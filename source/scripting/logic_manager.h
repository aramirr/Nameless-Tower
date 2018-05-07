#pragma once 
class LogicManager
{


public:

	LogicManager();
	void printDbg(const char* msg);
	void appearEntity(const char* name);
	void disappearEntity(const char* name);
	void activateCamera(const char* name);
	void disactivateCamera(const char* name);

	void setAmbientAdjustment(float value);
	void playLastCinematic();

	void activateText(const char* name);
	void disactivateText(const char* name);

};


class LogicPlayer
{


public:

	LogicPlayer();
	VEC3 getPlayerPosition();
	void setPlayerPosition(float x, float y, float z);

};