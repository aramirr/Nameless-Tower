#pragma once

#include "gui/gui_widget.h"

namespace GUI
{
  class CText : public CWidget
  {
  public:
    CText() = default;
    void render() override;

  private:
    TTextParams _textParams;

    friend class CParser;
  };
}