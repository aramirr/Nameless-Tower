#pragma once 
class LogicManager
{


public:

	LogicManager();
	void printDbg();
	void appearEntity(const char* name);
	void disappearEntity(const char* name);
	void activateCamera(const char* name);
	void disactivateCamera(const char* name);

};