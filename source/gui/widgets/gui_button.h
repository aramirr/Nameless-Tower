#pragma once

#include "gui/gui_widget.h"

namespace GUI
{
  class CButton : public CWidget
  {
  public:
    CButton() = default;
    void render() override;

  private:
    enum EState { ST_Idle = 0, ST_Selected, ST_Pressed, NUM_STATES };
    
    TButtonParams _states[NUM_STATES];
    EState _currentState = ST_Idle;

    friend class CParser;
  };
}