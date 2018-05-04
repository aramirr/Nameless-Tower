#pragma once

// -----------------------------------------------------
#include "utils/data_provider.h"
#include "render/mesh//mesh_loader.h"
#include "resources/resource.h"

class CCollisionMesh : public IResource {
public:
  TMeshLoader mesh;
  bool load(const std::string& name);
};
