#include "mcv_platform.h"
#include "game_core_skeleton.h"

#pragma comment(lib, "cal3d.lib" )

// ----------------------------------------------
class CGameCoreSkeletonResourceClass : public CResourceClass {
public:
  CGameCoreSkeletonResourceClass() {
    class_name = "Skeletons";
    extensions = { ".skeleton" };
  }
  IResource* create(const std::string& name) const override {
    dbg("Creating skelton resource %s\n", name.c_str());
    CGameCoreSkeleton* res = new CGameCoreSkeleton(name);
    bool is_ok = res->create(name);
    assert(is_ok);
    return res;
  }
};

// A specialization of the template defined at the top of this file
// If someone class getResourceClassOf<CGameCoreSkeletonResourceClass>, use this function:
template<>
const CResourceClass* getResourceClassOf<CGameCoreSkeleton>() {
  static CGameCoreSkeletonResourceClass the_resource_class;
  return &the_resource_class;
}



bool CGameCoreSkeleton::create(const std::string& res_name) {

  std::string name = "cally";

  // Read the core skeleton
  std::string csf = "data/skeletons/" + name + "/" + name + ".csf";
  bool is_ok = loadCoreSkeleton(csf);
  if (!is_ok)
    return false;

  // Read the core mesh
  std::string cmf = "data/skeletons/" + name + "/" + name + ".cmf";
  mesh_id = loadCoreMesh(cmf);
  if (mesh_id < 0)
    return false;

  // Read one anim
  std::string caf = "data/skeletons/" + name + "/jog.caf";
  anim_jog_id = loadCoreAnimation(caf);
  if (anim_jog_id < 0)
    return false;

  return true;
}


