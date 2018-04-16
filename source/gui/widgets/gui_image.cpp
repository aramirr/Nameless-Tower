#include "mcv_platform.h"
#include "gui/widgets/gui_image.h"

using namespace GUI;

void CImage::render()
{
  Engine.get().getGUI().renderTexture(_absolute, _imageParams._texture);
}
