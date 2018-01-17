#pragma once

#include "module.h"

class CModuleRender : public IModule
{
public:
	CModuleRender(const std::string& name);
  bool start() override;
  bool stop() override;
  void update(float delta) override;
  void render() override;

  void configure(int xres, int yres);
  void setBackgroundColor(float r, float g, float b, float a);

private:
  int _xres;
  int _yres;
  float _backgroundColor[4];
};