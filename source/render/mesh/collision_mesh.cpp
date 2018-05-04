#include "mcv_platform.h"
#include "collision_mesh.h"

// ----------------------------------------------
class CCollisionMeshResourceClass : public CResourceClass {
public:
  CCollisionMeshResourceClass() {
    class_name = "Collision Meshes";
    extensions = { ".col_mesh" };
  }
  IResource* create(const std::string& name) const override {
    // Create the new object & init it from the file source
    auto c = new CCollisionMesh();
    bool is_ok = c->load(name);
    assert(is_ok);
    return c;
  }
};

// ----------------------------------------------
template<>
const CResourceClass* getResourceClassOf<CCollisionMesh>() {
  static CCollisionMeshResourceClass the_resource_class;
  return &the_resource_class;
}

// ----------------------------------------------
bool CCollisionMesh::load(const std::string& name) {
  // Read the file
  CFileDataProvider fdp(name.c_str());
  if (!fdp.isValid())
    return false;
  // init it from the file source
  bool is_ok = mesh.load(fdp);
  assert(is_ok);
  assert(strcmp(mesh.header.vertex_type_name, "Pos") == 0);
  return true;
}

