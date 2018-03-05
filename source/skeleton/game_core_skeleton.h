#ifndef INC_GAME_CORE_SKELETON_H_
#define INC_GAME_CORE_SKELETON_H_

#include "resources/resource.h"
#include "cal3d/cal3d.h"

// This is the CalCoreModel + IResource interface + other metadata
// we might need in the future and is shared between all instances
// of this model
class CGameCoreSkeleton : public IResource, public CalCoreModel {
  int mesh_id;
  int anim_jog_id;
public:
  CGameCoreSkeleton( const std::string& aname ) : CalCoreModel(aname) {}
  bool create(const std::string& res_name);
};

#endif

