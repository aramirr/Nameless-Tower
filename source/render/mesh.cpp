#include "mcv_platform.h"
#include "mesh.h"

bool CRenderMesh::create(
  const void* vertex_data,
  size_t      num_bytes,
  const std::string& vtx_decl_name,
  eTopology new_topology
) {
  HRESULT hr;

  assert(vertex_data != nullptr);
  assert(num_bytes > 0);
  assert(new_topology != eTopology::UNDEFINED);

  // Save the parameter
  topology = new_topology;

  // Prepare a struct to create the buffer in gpu memory
  D3D11_BUFFER_DESC bd;
  ZeroMemory(&bd, sizeof(bd));
  bd.Usage = D3D11_USAGE_DEFAULT;
  bd.ByteWidth = (UINT)num_bytes;
  bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  bd.CPUAccessFlags = 0;

  // This is the initial data
  D3D11_SUBRESOURCE_DATA InitData;
  ZeroMemory(&InitData, sizeof(InitData));
  InitData.pSysMem = vertex_data;
  hr = Render.device->CreateBuffer(&bd, &InitData, &vb);
  if (FAILED(hr))
    return false;

  vtx_decl = CVertexDeclManager::get().getByName(vtx_decl_name);
  assert(vtx_decl);
  assert(vtx_decl->bytes_per_vertex > 0);

  num_vertexs = (UINT)(num_bytes / vtx_decl->bytes_per_vertex);
  assert(num_vertexs * vtx_decl->bytes_per_vertex == num_bytes);

  return true;
}

void CRenderMesh::destroy() {
  SAFE_RELEASE(vb);
}

void CRenderMesh::activate() {
  assert(vb);
  assert(vtx_decl);

  // Set vertex buffer based on my vertex type
  UINT stride = vtx_decl->bytes_per_vertex;
  UINT offset = 0;
  Render.ctx->IASetVertexBuffers(0, 1, &vb, &stride, &offset);

  // Set primitive topology
  Render.ctx->IASetPrimitiveTopology( (D3D11_PRIMITIVE_TOPOLOGY)topology );
}

void CRenderMesh::render() {
  Render.ctx->Draw(num_vertexs, 0);
}

void CRenderMesh::activateAndRender() {
  activate();
  render();
}