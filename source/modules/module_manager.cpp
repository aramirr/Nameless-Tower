#include "mcv_platform.h"
#include "module_manager.h"

bool CModuleManager::start()
{
  return startModules(_system_modules);
}

bool CModuleManager::stop() 
{
  bool ok = true;
  ok &= stopModules(_active_modules);
  ok &= stopModules(_system_modules);
  return ok;
}

// Dispatch the OS msg to all modules registered as system module
LRESULT CModuleManager::OnOSMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
  bool processed = false;
  for (auto& mod : _system_modules)
  {
    processed |= (mod->OnOSMsg(hWnd, msg, wParam, lParam ) != 0);
  }
  if (processed)
    return 1;
  return 0;
}

void CModuleManager::update(float delta)
{
  for (auto& mod : _system_modules)
  {
    mod->update(delta);
  }
  for (auto& mod : _active_modules)
  {
    mod->update(delta);
  }
}

void CModuleManager::render()
{
  for (auto& mod : _system_modules)
  {
    mod->render();
  }
  for (auto& mod : _active_modules)
  {
    mod->render();
  }
}

void CModuleManager::registerSystemModule(IModule* mod)
{
  _registered_modules.push_back(mod);
  _system_modules.push_back(mod);
}

void CModuleManager::registerGameModule(IModule* mod)
{
  _registered_modules.push_back(mod);
}


void CModuleManager::registerGameState(CGameState* gs)
{
  _gamestates.push_back(gs);
}

CGameState* CModuleManager::getGameState(const std::string& gsName)
{
  auto it = std::find_if(_gamestates.begin(), _gamestates.end(), [&](const CGameState* gs) {
    return gs->getName() == gsName;
  });
  if (it != _gamestates.end())
  {
    return *it;
  }
  return nullptr;
}

void CModuleManager::changeGameState(const std::string& gsName)
{
  // stop current game modules
  stopModules(_active_modules);

  // clear game modules
  _active_modules.clear();

  // get requested gamestate
  CGameState* gs = getGameState(gsName);
  assert(gs);

  // add new gamestate modules
  for (auto& mod : *gs)
  {
    _active_modules.push_back(mod);
  }

  // start new gamestate modules
  startModules(_active_modules);
}

bool CModuleManager::startModules(VModules& modules)
{
  bool ok = true;
  for (auto& mod : modules)
  {
    ok &= mod->start();
  }
  return ok;
}

bool CModuleManager::stopModules(VModules& modules)
{
  bool ok = true;
  for (auto& mod : modules)
  {
    ok &= mod->stop();
  }
  return ok;
}