#include "mcv_platform.h"
#include "module_manager.h"
#include "utils/json.hpp"
#include <fstream>

// for convenience
using json = nlohmann::json;

bool CModuleManager::start()
{
  return startModules(_system_modules);
}

bool CModuleManager::stop() 
{
  bool ok = true;
	if (_current_gs)
	{
		ok &= stopModules(*_current_gs);
		_current_gs = nullptr;
	}
  ok &= stopModules(_system_modules);

	for (auto& gs : _gamestates)
	{
		delete gs;
	}
	_gamestates.clear();

  return ok;
}

void CModuleManager::update(float delta)
{
	for (auto& mod : _update_modules)
	{
		if (mod->isActive()) 
		{
			mod->update(delta);
		}
	}
}

void CModuleManager::render()
{
	for (auto& mod : _render_modules)
	{
		if (mod->isActive())
		{
			mod->render();
		}
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

IModule* CModuleManager::getModule(const std::string& modName)
{
	auto it = std::find_if(_registered_modules.begin(), _registered_modules.end(), [&](const IModule* mod) {
		return mod->getName() == modName;
	});
	if (it != _registered_modules.end())
	{
		return *it;
	}
	return nullptr;
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
	if (_current_gs)
	{
		stopModules(*_current_gs);
	}

  // get requested gamestate
  CGameState* gs = getGameState(gsName);
  assert(gs);
	
  // start new gamestate modules
  startModules(*gs);
	_current_gs = gs;
}

bool CModuleManager::startModules(VModules& modules)
{
  bool ok = true;
  for (auto& mod : modules)
  {
    ok &= mod->start();
		if (ok)
		{
			mod->setActive(true);
		}
  }
  return ok;
}

bool CModuleManager::stopModules(VModules& modules)
{
  bool ok = true;
  for (auto& mod : modules)
  {
    ok &= mod->stop();
		if (ok)
		{
			mod->setActive(false);
		}
  }
  return ok;
}

void CModuleManager::loadModules(const std::string& filename)
{
	std::ifstream file_json(filename);
	json json_data;
	file_json >> json_data;

	// parse update modules
	dbg("UPDATE\n");
	auto& json_update = json_data["update"];
	for (auto& modName : json_update)
	{
		std::string& str = modName.get<std::string>();
		IModule* mod = getModule(str);
		if (mod)
		{
			_update_modules.push_back(mod);
		}
		else
		{
			fatal("Unkown update module '%s'\n", str.c_str());
		}
	}

	// parse render modules
	dbg("RENDER\n");
	auto& json_render = json_data["render"];
	for (auto& modName : json_render)
	{
		std::string& str = modName.get<std::string>();
		IModule* mod = getModule(str);
		if (mod)
		{
			_render_modules.push_back(mod);
		}
		else
		{
			fatal("Unkown render module '%s'\n", str.c_str());
		}
	}
}

void CModuleManager::loadGamestates(const std::string& filename)
{
	std::ifstream file_json(filename);
	json json_data;
	file_json >> json_data;

	// parse gamestates
	dbg("GAMESTATES\n");
	auto& json_gametates = json_data["gamestates"];
	for (auto& json_gs : json_gametates)
	{
		std::string gs_name = json_gs["name"].get<std::string>();
		auto& json_modules = json_gs["modules"];

		CGameState* newGs = new CGameState(gs_name);

		for (auto& json_mod : json_modules)
		{
			std::string& str = json_mod.get<std::string>();
			IModule* mod = getModule(str);
			if (mod)
			{
				newGs->push_back(mod);
			}
			else
			{
				fatal("Unkown module '%s'\n", str.c_str());
			}
		}

		registerGameState(newGs);
	}
}