#include "mcv_platform.h"
#include "gui/gui_parser.h"
#include "gui/widgets/gui_image.h"
#include "gui/widgets/gui_text.h"
#include "gui/widgets/gui_bar.h"
#include "gui/widgets/gui_button.h"

using namespace GUI;

void CParser::parseFile(const std::string& filename)
{
  std::ifstream file_json(filename);
  json json_data;
  file_json >> json_data;

  for (auto& jWidget : json_data)
  {
    parseWidget(jWidget);
  }
}

void CParser::parseWidget(json& data, CWidget* parent)
{
  const std::string& name = data["name"];
  const std::string& type = data["type"];
  CWidget* wdgt = nullptr;

  if (type == "image")
  {
    CImage* image = new CImage;
    parseImageParams(image->_imageParams, data["image_params"]);
    wdgt = image;
  }
  else if (type == "text")
  {
    CText* text = new CText;
    parseTextParams(text->_textParams, data["text_params"]);
    wdgt = text;
  }
  else if (type == "bar")
  {
    CBar* bar = new CBar;
    parseImageParams(bar->_imageParams, data["image_params"]);
    parseBarParams(bar->_barParams, data["bar_params"]);
    wdgt = bar;
  }
  else if (type == "button")
  {
    CButton* button = new CButton;
    for (int i = 0; i < CButton::NUM_STATES; ++i)
    {
      parseImageParams(button->_states[i]._imageParams, data["image_params"]);
      parseTextParams(button->_states[i]._textParams, data["text_params"]);
    }
    wdgt = button;
  }
  else if (type == "widget")
  {
    wdgt = new CWidget;
  }

  assert(wdgt);
  if (wdgt)
  {
    wdgt->_name = name;
    parseParams(wdgt->_params, data["params"]);
    Engine.getGUI().registerWidget(wdgt);

    if (parent)
    {
      parent->addChild(wdgt);
    }


    // parse widget children
    for (auto& jChild : data["children"])
    {
      parseWidget(jChild, wdgt);
    }

    wdgt->computeAbsolute();
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
  params._minUV = loadVEC2(data["minUV"]);
  params._maxUV = loadVEC2(data["maxUV"]);
  const std::string& textureFile = data["texture"];
  params._texture = Resources.get(textureFile)->as<CTexture>();
}

void CParser::parseTextParams(TTextParams& params, json& data)
{
  params._color = loadVEC4(data["color"]);
  params._size = data.value("size", 1.f);
  params._text = data.value("text", std::string());
  const std::string& alignment = data.value("alignment", "");
  if (alignment == "center")      params._alignment = TTextParams::Center;
  else if (alignment == "right")  params._alignment = TTextParams::Right;
  else                            params._alignment = TTextParams::Left;
}

void CParser::parseBarParams(TBarParams& params, json& data)
{
  params._variable = data.value("variable", std::string());
  params._direction = data["direction"] == "vertical" ? TBarParams::Vertical : TBarParams::Horizontal;
}
