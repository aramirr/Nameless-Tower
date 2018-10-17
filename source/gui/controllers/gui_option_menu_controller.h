#pragma once

#include "gui/gui_controller.h"

namespace GUI
{
  class CButton;

  class COptionMenuController : public CController
  {
  public:
    virtual void update(float delta) override;
    void registerOption(const std::string& name, GUICallback cb, int i);
    void setCurrentOption(int newOption);
		void setCurrentSection(int newSection);

    void setActive(bool _active);
		void changeResolution(int _x, int _y);
  private:
    struct TOption
    {
      CButton* button;
      GUICallback callback;
    };
    std::vector<std::vector<TOption>> _options;
    int _currentOption = 0;
		int _currentSection = 0;

    int getCurrentOption();

    bool carga = true;
    bool active = false;

  };
}
