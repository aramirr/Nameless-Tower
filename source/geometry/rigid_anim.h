#pragma once

#include "resources/resource.h"

namespace RigidAnims {

  class CRigidAnimResource;

  // ----------------------------------------------
  struct TKey {
    VEC3  pos;
    float scale;
    QUAT  rot;
  };

  // ----------------------------------------------
  struct CController {
    const CRigidAnimResource* anims = nullptr;
    int                       track_index = 0;
    std::string               track_name;
    // Returns true if it has finished
    bool sample(TKey* out_key, float t) const;
  };

  // ----------------------------------------------
  struct TTrackInfo {
    char name[64];
    int  first_key;
    int  num_key;
    float fps;
    float min_time;
    float max_time;
  };

  // ----------------------------------------------
  class CRigidAnimResource : public IResource {

    std::vector< TTrackInfo > infos;
    std::vector< TKey > keys;

  public:
    bool create(const std::string& name);
    void debugInMenu() override;
  };

}
