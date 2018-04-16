#pragma once

#include "gui_params.h"

namespace GUI
{
  class CImageWidget;

  class CParser
  {
  public:
    void parseFile(const std::string& file);
    void parseParams(TParams& params, json& data);
    void parseImageParams(TImageParams& params, json& data);
  };
}