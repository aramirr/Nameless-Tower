#pragma once

#include "modules/module.h"
#include "gui/gui_widget.h"
#include "utils/variant.h"
#include "gui/gui_controller.h"
#include "gui/controllers/gui_main_menu_controller.h"
#include "gui/controllers/gui_omnidash_arrow_controller.h"
#include "gui/controllers/gui_pause_menu_controller.h"
#include "gui/controllers/gui_option_menu_controller.h"
#include "gui/controllers/gui_keyboard_menu_controller.h"

using namespace GUI;


class CModuleGUI : public IModule
{
public:
  CModuleGUI(const std::string& name);
  bool start() override;
  bool stop() override;
  void update(float delta) override;
  void renderGUI() override;

  //FUNCIONES GENERICAS PARA SER LLAMADAS DESDE DIFERENTES LUGARES DEL ENGINE
  void desactiveMainMenu();
  void activeMainMenu();
  void desactivePauseMenu();
  void activePauseMenu();
	void resetPauseMenu();
	void desactiveOptionMenu();
	void activeOptionMenu();
	void desactiveKeyboardMenu();
	void activeKeyboardMenu();
  void setOmindash(bool omnidash);

	void setFullScreenOption(int x, int y);
	void setResolutionOption(int x, int y);
	void setVolumenOption(int x, int y);
	void setVsyncOption(int x, int y);

	void changeResolution(int _x, int _y);
	void fullScreen(bool _fullscreen);

  // widget management
  void registerWidget(GUI::CWidget* wdgt);
  GUI::CWidget* getWidget(const std::string& name, bool recursive = false) const;
  void activateWidget(const std::string& name);
  void desactivateWidget(const std::string& name);


  // controller management
  void registerController(GUI::CController* controller);
  void unregisterController(GUI::CController* controller);

  CCamera& getCamera();
  MVariants& getVariables();

  void renderTexture(const MAT44& world, const CTexture* texture, const VEC2& minUV, const VEC2& maxUV, const VEC4& color);
  void renderText(const MAT44& world, const std::string& text, const VEC4& color);
	void activateSplash() { splash = true; }
	bool getSplash() { return splash; }

private:
	float timer = 3.f;
	float firstdt;
	bool firstTime;
	bool splash;
	bool nvidia;
	bool upf;
	bool mainMenu;

  CCamera _orthoCamera;
  const CRenderTechnique* _technique = nullptr;
  const CRenderMesh* _quadMesh = nullptr;
  const CTexture* _fontTexture = nullptr;
  
  CMainMenuController* mmc;
  CPauseMenuController* pmc;
	COptionMenuController* omc;
	CKeyboardMenuController* kmc;
  COmnidashArrowController* odc;

  GUI::VWidgets _registeredWidgets;
  GUI::VWidgets _activeWidgets;
  MVariants _variables;
  GUI::VControllers _controllers;
};
