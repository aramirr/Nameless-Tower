#include "mcv_platform.h"
#include "module_gui.h"
#include "render/render_objects.h"
#include "gui/gui_parser.h"

using namespace GUI;

CModuleGUI::CModuleGUI(const std::string& name)
	: IModule(name)
{}

bool CModuleGUI::start()
{
  const float width = 1920;
  const float height = 1080;
  _orthoCamera.setOrthographic(width, height);

  _technique = Resources.get("gui.tech")->as<CRenderTechnique>();
  _quadMesh = Resources.get("unit_quad_xy.mesh")->as<CRenderMesh>();

  CParser parser;
  parser.parseFile("data/gui/test.json");
  /*parser.parseFile("data/gui/main_menu.json");
  parser.parseFile("data/gui/gameplay.json");
  parser.parseFile("data/gui/game_over.json");*/

  activateWidget("test_image");

  return true;
}

bool CModuleGUI::stop()
{
  return true;
}


void CModuleGUI::update(float delta)
{
  for (auto& wdgt : _activeWidgets)
  {
    wdgt->updateAll(delta);
  }
}

void CModuleGUI::renderGUI()
{
  for (auto& wdgt : _activeWidgets)
  {
    wdgt->renderAll();
  }
}

void CModuleGUI::registerWidget(CWidget* wdgt)
{
  _registeredWidgets.push_back(wdgt);
}

CWidget* CModuleGUI::getWidget(const std::string& name) const
{
  for (auto& wdgt : _registeredWidgets)
  {
    if (wdgt->getName() == name)
    {
      return wdgt;
    }
  }
  return nullptr;
}

void CModuleGUI::activateWidget(const std::string& name)
{
  CWidget* wdgt = getWidget(name);
  if (wdgt)
  {
    _activeWidgets.push_back(wdgt);
  }
}

CCamera& CModuleGUI::getCamera()
{
  return _orthoCamera;
}

void CModuleGUI::renderTexture(const MAT44& world, const CTexture* texture)
{
  //auto* tech = Resources.get("gui.tech")->as<CRenderTechnique>();
  //auto* texture = Resources.get("data/textures/fieldstone-c.dds")->as<CTexture>();
  //auto* mesh = Resources.get("unit_quad_xy.mesh")->as<CRenderMesh>();

  //MAT44 tr = MAT44::CreateTranslation(10, 50, 0);
  //MAT44 rot = MAT44::CreateFromYawPitchRoll(0, 0, 0);
  //MAT44 sc = MAT44::CreateScale(480, 320, 1);
  //MAT44 world = sc * rot * tr;

  assert(_technique && _quadMesh);

  cb_object.obj_world = world;
  cb_object.obj_color = VEC4(1, 1, 1, 1);
  cb_object.updateGPU();

  _technique->activate();
  if (texture)
    texture->activate(TS_ALBEDO);

  _quadMesh->activateAndRender();
}

