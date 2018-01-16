#pragma once

class IModule
{
public:
  virtual bool start() { return true; }
  virtual bool stop() { return true; }
  virtual void update(float delta) { (void)delta; }
  virtual void render() {}
};

using VModules = std::vector<IModule*>;
