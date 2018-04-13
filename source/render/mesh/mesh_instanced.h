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
  virtual void uploadInstancesToGPU() = 0;
  virtual void clearInstances() = 0;
};

// -------------------------------------------------------------------
template< typename TInstance >
class CRenderMeshInstanced : public CRenderMeshInstancedBase {

  void allocGPUIfRequired(size_t new_max_instances) {
    
    if (new_max_instances > num_instances_allocated_in_gpu) {

      // Do we have an initial value?, start from 4
      if (!num_instances_allocated_in_gpu)
        num_instances_allocated_in_gpu = 4;

      // Keep increasing in powers of 2
      while (new_max_instances > num_instances_allocated_in_gpu)
        num_instances_allocated_in_gpu *= 2;

      if (isValid())
        destroy();

      std::string decl_name = TInstance::getDeclName();

      // Create the VB as a dynamic buffer to hold a maximum of N instances
      bool is_ok = CRenderMeshInstancedBase::create(
        cpu_instances.data(),
        num_instances_allocated_in_gpu * sizeof(TInstance),  // Bytes required
        decl_name.c_str(),
        CRenderMesh::POINT_LIST,
        nullptr, 0, 0,            // No index data
        nullptr,                  // No specify group
        true                      // is dynamic
      );

    }

  }

public:
  std::vector< TInstance > cpu_instances;

  // Updates gpu and configures the group 0 to render the current number of instances
  void uploadInstancesToGPU() override {
    allocGPUIfRequired(cpu_instances.capacity());
    updateFromCPU(cpu_instances.data(), cpu_instances.size() * sizeof(TInstance));
    setSubGroupSize(0, (uint32_t) cpu_instances.size());
  }

  void clearInstances() {
    cpu_instances.clear();
  }

};


#endif
