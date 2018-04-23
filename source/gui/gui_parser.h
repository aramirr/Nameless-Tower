#pragma once

#include "gui_params.h"

namespace GUI
{
  class CImageWidget;

  class CParser
  {
  public:
    void parseFile(const std::string& file);
    void parseWidget(json& data, CWidget* parent = nullptr);
    void parseParams(TParams& params, json& data);
    void parseImageParams(TImageParams& params, json& data);
    void parseTextParams(TTextParams& params, json& data);
    void parseBarParams(TBarParams& params, json& data);
  };
}