#ifndef INC_RENDER_MESH_H_
#define INC_RENDER_MESH_H_

class CVertexDecl;

class CRenderMesh {

public:

  enum eTopology {
    UNDEFINED = D3D_PRIMITIVE_TOPOLOGY_UNDEFINED,
    TRIANGLE_LIST = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
  };

  bool create(
    const void* vertex_data,
    size_t      num_bytes,
    const std::string& vtx_decl_name,
    eTopology new_topology
  );
  void destroy();
  void activate();
  void render();
  void activateAndRender();

private:
  
  ID3D11Buffer*      vb = nullptr;
  const CVertexDecl* vtx_decl = nullptr;
  eTopology          topology = eTopology::UNDEFINED;
  UINT               num_vertexs = 0;
};


#endif

