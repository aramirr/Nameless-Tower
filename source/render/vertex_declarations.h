#ifndef INC_RENDER_VERTEX_DECLARATIONS_H_
#define INC_RENDER_VERTEX_DECLARATIONS_H_

class CVertexDecl {

  ID3D11InputLayout*        vertex_layout = nullptr;
  D3D11_INPUT_ELEMENT_DESC* cpu_layout = nullptr;
  UINT numElements = 0;
  std::string name;

  bool create(ID3DBlob* vs_blob) {
    HRESULT hr = Render.device->CreateInputLayout(cpu_layout, numElements, vs_blob->GetBufferPointer(),
      vs_blob->GetBufferSize(), &vertex_layout);
    return !FAILED(hr);
  }

};

class CVertexDeclManager {

  std::map< std::string, CVertexDecl* > decls;

public:

  static CVertexDeclManager& get();

  bool create();
  const CVertexDecl* getByName(const std::string& name);
};


#endif

