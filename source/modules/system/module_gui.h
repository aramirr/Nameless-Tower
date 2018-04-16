#pragma once

#include "modules/module.h"
#include "gui/gui_widget.h"

class CModuleGUI : public IModule
{
public:
  CModuleGUI(const std::string& name);
  bool start() override;
  bool stop() override;
  void update(float delta) override;
  void renderGUI() override;

  // widget management
  void registerWidget(GUI::CWidget* wdgt);
  GUI::CWidget* getWidget(const std::string& name) const;
  void activateWidget(const std::string& name);

  CCamera& getCamera();

  void renderTexture(const MAT44& world, const CTexture* texture);

private:
  CCamera _orthoCamera;
  const CRenderTechnique* _technique = nullptr;
  const CRenderMesh* _quadMesh = nullptr;
  
  GUI::VWidgets _registeredWidgets;
  GUI::VWidgets _activeWidgets;
};
