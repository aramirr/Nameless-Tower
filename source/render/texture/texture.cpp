#include "mcv_platform.h"
#include "texture.h"

// ----------------------------------------------
class CTexturesResourceClass : public CResourceClass {
public:
  CTexturesResourceClass() {
    class_name = "Textures";
    extension = ".dds";
  }
  IResource* create(const std::string& name) const override {
    dbg("Creating texture %s\n", name.c_str());
    IResource* res = new CTexture();
    return res;
  }
};

// A specialization of the template defined at the top of this file
// If someone class getResourceClassOf<CTexture>, use this function:
template<>
const CResourceClass* getResourceClassOf<CTexture>() {
  static CTexturesResourceClass the_textures_resource_class;
  return &the_textures_resource_class;
}

void CTexture::debugInMenu() {
  ImGui::Text("Nothing to show");
  // ...
}
