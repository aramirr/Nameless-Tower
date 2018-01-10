#include "mcv_platform.h"

static CVertexDeclManager vtx_decl_manager;

CVertexDeclManager& CVertexDeclManager::get() {
  return vtx_decl_manager;
}

// ----------------------------------------------------------
CVertexDecl* CVertexDeclManager::createNew(
  const std::string& name
  , D3D11_INPUT_ELEMENT_DESC* layout
  , UINT numElements
) {
  CVertexDecl* decl = new CVertexDecl();
  decl->name = name; 
  decl->cpu_layout = layout;
  decl->numElements = numElements;
  decls[name] = decl;
  return decl;
}

// ----------------------------------------------------------
bool CVertexDeclManager::create() {

  // Create & register each vertex declr

  // Define the input layout
  {
    static D3D11_INPUT_ELEMENT_DESC layout[] = {
      { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    CVertexDecl* decl = createNew("Pos", layout, ARRAYSIZE(layout));
    decl->bytes_per_vertex = 3 * 4;
  }

  {
    static D3D11_INPUT_ELEMENT_DESC layout[] = {
      { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
      { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    CVertexDecl* decl = createNew("PosClr", layout, ARRAYSIZE(layout));
    decl->bytes_per_vertex = 3 * 4 + 4 * 4;
  }

  return true;
}

const CVertexDecl* CVertexDeclManager::getByName(const std::string& name) {
  auto it = decls.find(name);
  if (it == decls.end()) {
    fatal("Can't find vertex declaration named '%s'", name.c_str());
    return nullptr;
  }
  return it->second;
}


