#ifndef INC_GAME_CORE_SKELETON_H_
#define INC_GAME_CORE_SKELETON_H_

#include "resources/resource.h"
#include "cal3d/cal3d.h"

struct TSkinVertex {
  VEC3 pos;
  VEC3 normal;
  VEC2 uv;
  VEC4 tangent;
  uint8_t bone_ids[4];
  uint8_t bone_weights[4];    // 0.255   -> 0..1
};

// This is the CalCoreModel + IResource interface + other metadata
// we might need in the future and is shared between all instances
// of this model
class CGameCoreSkeleton : public IResource, public CalCoreModel {
  std::string root_path;

  bool convertCalCoreMesh2RenderMesh(CalCoreMesh* cal_mesh, const std::string& ofilename);

  // ---------------------------------------------------
public:
  CGameCoreSkeleton( const std::string& aname ) : CalCoreModel(aname) {}
  bool create(const std::string& res_name);
  void debugInMenu() override;
  float              bone_ids_debug_scale = 1.f;
  std::vector< int > bone_ids_to_debug;

};

#endif

