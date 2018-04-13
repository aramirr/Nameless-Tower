#include "mcv_platform.h"
#include "mesh_instanced.h"

// Configure the two streams and send the mesh to render
void CRenderMeshInstancedBase::renderSubMesh(uint32_t sub_group_idx) const {

  assert(isValid());
  assert(instanced_mesh);
  assert(instanced_mesh->isValid());
  assert(sub_group_idx < subgroups.size());

  auto& sb = subgroups[sub_group_idx];
  if (sb.num_indices == 0)
    return;

  // Set the source of both streams
  ID3D11Buffer* vbs[2] = {
    instanced_mesh->getVB(),      // 0 -> Instanced mesh
    getVB()                       // 1 -> Instances data
  };

  // Saltos entre vertices para cada stream
  UINT strides[2] = {
    instanced_mesh->getVertexDecl()->bytes_per_vertex,
    getVertexDecl()->bytes_per_vertex
  };

  // Send vertexs from the start of each vertex buffer
  UINT offsets[2] = {
    0,
    0
  };
  Render.ctx->IASetVertexBuffers(0, 2, vbs, strides, offsets);

  // Set primitive topology
  Render.ctx->IASetPrimitiveTopology((D3D_PRIMITIVE_TOPOLOGY)instanced_mesh->getTopology());

  // Is Indexed?
  if (instanced_mesh->getIB()) {
    instanced_mesh->activateIndexBuffer();
    Render.ctx->DrawIndexedInstanced(instanced_mesh->getIndicesCount(), sb.num_indices, 0, 0, 0);
  }
  else {
    Render.ctx->DrawInstanced(instanced_mesh->getVertexsCount(), sb.num_indices, 0, 0);
  }

}