#pragma once

#include "gui/gui_controller.h"

namespace GUI
{
  class COmnidashArrowController : public CController
  {
  public:
    virtual void update(float delta) override;

    void setActive(bool _active);

  private:
    bool active = false;;
  };
}
