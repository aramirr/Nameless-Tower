#include "mcv_platform.h"
#include "gui/gui_parser.h"
#include "gui/widgets/gui_image.h"

using namespace GUI;

void CParser::parseFile(const std::string& filename)
{
  std::ifstream file_json(filename);
  json json_data;
  file_json >> json_data;

  for (auto& jWidget : json_data)
  {
    const std::string& name = jWidget["name"];
    const std::string& type = jWidget["type"];
    if (type == "image")
    {
      CImage* wdgt = new CImage;
      wdgt->_name = name;
      parseParams(wdgt->_params, jWidget["params"]);
      parseImageParams(wdgt->_imageParams, jWidget["image_params"]);
      wdgt->computeAbsolute();
      Engine.getGUI().registerWidget(wdgt);
    }
  }
}

void CParser::parseParams(TParams& params, json& data)
{
  params._visible = data.value("visible", true);
  params._position = loadVEC2(data["position"]);
  params._rotation = deg2rad(data.value("rotation", 0.f));
  params._scale = loadVEC2(data["scale"]);
  params._size = loadVEC2(data["size"]);
}

void CParser::parseImageParams(TImageParams& params, json& data)
{
  params._color = loadVEC4(data["color"]);
  const std::string& textureFile = data["texture"];
  params._texture = Resources.get(textureFile)->as<CTexture>();
}
