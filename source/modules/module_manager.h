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

  void registerSystemModule(IModule* mod);
  void registerGameModule(IModule* mod);

  void registerGameState(CGameState* gs);
  void changeGameState(const std::string& gsName);
  CGameState* getGameState(const std::string& gsName);

  LRESULT OnOSMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
  bool startModules(VModules& modules);
  bool stopModules(VModules& modules);

  VModules _registered_modules;
  VModules _system_modules;
  VModules _active_modules;

  VGameStates _gamestates;
};