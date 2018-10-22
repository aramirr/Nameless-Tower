#pragma once

#include "gui/gui_controller.h"

namespace GUI
{
  class CButton;

  class CKeyboardMenuController : public CController
  {
  public:
    virtual void update(float delta) override;
    void registerOption(const std::string& name, GUICallback cb);
    void setCurrentOption(int newOption);
		void setActive(bool _active);
  private:
    struct TOption
    {
      CButton* button;
      GUICallback callback;
    };
    std::vector<TOption> _options;
    int _currentOption = 0;

    int getCurrentOption();

		float timer;

    bool carga = true;
		bool active = false;
  };
}
