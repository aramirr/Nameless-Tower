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
		void fullScreen(bool _fulscreen);
		void setFullScreenOption(int x, int y);
		void setResolutionOption(int x, int y);
		void setVolumenOption(int x, int y);
		void setVsyncOption(int x, int y);
  private:
    struct TOption
    {
      CButton* button;
      GUICallback callback;
    };
    std::vector<std::vector<TOption>> _options;
    int _currentOption = 0;
		int _currentSection = 0;

		VEC2 fullScreenOption;
		VEC2 resolutionOption;
		VEC2 volumenOption;
		VEC2 vsyncOption;

    int getCurrentOption();

    bool carga = true;
    bool active = false;

		bool loading = true;

  };
}
