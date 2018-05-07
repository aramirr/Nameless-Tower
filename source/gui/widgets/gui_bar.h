#pragma once

#include "gui/gui_widget.h"

namespace GUI
{
  class CBar : public CWidget
  {
  public:
    CBar() = default;
    void render() override;
    TImageParams* getImageParams() override;

  private:
    TImageParams _imageParams;
    TBarParams _barParams;

    friend class CParser;
  };
}