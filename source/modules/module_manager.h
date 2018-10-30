#pragma once

#include "module.h"
#include "gamestate.h"

class CModuleManager
{
public:
  bool start();
  bool stop();
  void update(float delta);
	void render();
  void renderGUI();

  void registerSystemModule(IModule* mod);
  void registerGameModule(IModule* mod);
	IModule* getModule(const std::string& gsName);

  void registerGameState(CGameState* gs);
  void changeGameState(const std::string& gsName);
	CGameState* getGameState(const std::string& gsName);

	void loadModules(const std::string& filename);
	void loadGamestates(const std::string& filename);

  LRESULT OnOSMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

  bool stopModules(VModules& modules);
  bool startModules(VModules& modules);

private:
	void applyRequestedGameState();
	void renderDebug();

	bool startModule(IModule* mod);
	bool stopModule(IModule* mod);     

  VModules _registered_modules;
  VModules _system_modules;
	VModules _update_modules;
	VModules _render_modules;

  VGameStates _gamestates;
	CGameState* _current_gs = nullptr;
	CGameState* _startup_gs = nullptr;
	CGameState* _requested_gs = nullptr;
};