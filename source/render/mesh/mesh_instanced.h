#ifndef INC_RENDER_MESHES_INSTANCED_MESH_H_
#define INC_RENDER_MESHES_INSTANCED_MESH_H_

#include "mesh.h"

// -------------------------------------------------------------------
// I' will hold the instances data. instanced_mesh is
// a pointer to the other data, the billboard
// This part is independent of the type of instance of the mesh, which has
// been moved to a .cpp file
class CRenderMeshInstancedBase : public CRenderMesh {
protected:
  const CRenderMesh* instanced_mesh = nullptr;    // Each vertex represents an instance
  size_t             num_instances_allocated_in_gpu = 0;

public:
  void setInstancedMesh(const CRenderMesh* new_instanced_mesh) {
    instanced_mesh = new_instanced_mesh;
  }
  void renderSubMesh(uint32_t sub_group_idx) const override;
  void reserveGPUInstances(size_t new_max_instances);
  virtual void uploadInstancesToGPU() = 0;
};

// -------------------------------------------------------------------
// This is a version of the mesh associated to a specific instance type
// TInstance could be sprites, meshes, decals
// Instance defines the specific data associated to each instance
template< typename TInstance >
class CRenderMeshInstanced : public CRenderMeshInstancedBase {
public:
  std::vector< TInstance > cpu_instances;

  CRenderMeshInstanced() {
    // This provides access to the bytes_per_vertex and whole vertex declaration
    // of the instance
    vtx_decl = CVertexDeclManager::get().getByName(TInstance::getDeclName());
  }

  // Updates gpu and configures the group 0 to render the current number of instances
  void uploadInstancesToGPU() override {
    reserveGPUInstances(cpu_instances.capacity());
    updateFromCPU(cpu_instances.data(), cpu_instances.size() * sizeof(TInstance));
    setSubGroupSize(0, (uint32_t) cpu_instances.size());
  }

};


#endif
