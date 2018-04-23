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
  _fontTexture = Resources.get("data/textures/gui/font.dds")->as<CTexture>();

  CParser parser;
  parser.parseFile("data/gui/test.json");
  /*parser.parseFile("data/gui/main_menu.json");
  parser.parseFile("data/gui/gameplay.json");
  parser.parseFile("data/gui/game_over.json");*/

  activateWidget("test");

  _variables.setVariant("progress", 0.5f);

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

  // change bar value
  float value = _variables.getFloat("progress");
  if (EngineInput[VK_LEFT].isPressed())
  {
    value = clamp(value - 0.5f * delta, 0.f, 1.f);
  }
  if (EngineInput[VK_RIGHT].isPressed())
  {
    value = clamp(value + 0.5f * delta, 0.f, 1.f);
  }
  _variables.setVariant("progress", value);
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

MVariants& CModuleGUI::getVariables()
{
  return _variables;
}

void CModuleGUI::renderTexture(const MAT44& world, const CTexture* texture, const VEC2& minUV, const VEC2& maxUV, const VEC4& color)
{
  assert(_technique && _quadMesh);

  cb_object.obj_world = world;
  cb_object.obj_color = VEC4(1, 1, 1, 1);
  cb_object.updateGPU();

  cb_gui.minUV = minUV;
  cb_gui.maxUV = maxUV;
  cb_gui.tint_color = color;
  cb_gui.updateGPU();

  _technique->activate();
  if (texture)
    texture->activate(TS_ALBEDO);

  _quadMesh->activateAndRender();
}

void CModuleGUI::renderText(const MAT44& world, const std::string& text)
{
  assert(_fontTexture);

  int cellsPerRow = 8;
  float cellSize = 1.f / 8.f;
  char firstCharacter = ' ';
  for (size_t i = 0; i < text.size(); ++i)
  {
    char c = text[i];

    int cell = c - firstCharacter;
    int row = cell / cellsPerRow;
    int col = cell % cellsPerRow;

    VEC2 minUV = VEC2(col * cellSize, row * cellSize);
    VEC2 maxUV = minUV + VEC2(1, 1) * cellSize;
    VEC2 gap = i * VEC2(1, 0);
    MAT44 w = MAT44::CreateTranslation(gap.x, gap.y, 0.f) * world;

    renderTexture(w, _fontTexture, minUV, maxUV, VEC4(1, 1, 1, 1));
  }
}
