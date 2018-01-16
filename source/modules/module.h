#pragma once

class IModule
{
public:
  virtual bool start() { return true; }
  virtual bool stop() { return true; }
  virtual void update(float delta) { (void)delta; }
  virtual void render() {}

  virtual LRESULT OnOSMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) { return 0; }
};

using VModules = std::vector<IModule*>;
