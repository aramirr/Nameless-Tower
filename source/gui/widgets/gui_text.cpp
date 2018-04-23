#include "mcv_platform.h"
#include "gui_text.h"

using namespace GUI;

void CText::render()
{
  MAT44 w = MAT44::CreateScale(_textParams._size) * _absolute;
  Engine.get().getGUI().renderText(w, _textParams._text);
}
