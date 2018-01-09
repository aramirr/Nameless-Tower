#include "mcv_platform.h"

static CVertexDeclManager vtx_decl_manager;

static CVertexDeclManager& get() {
  return vtx_decl_manager;
}

bool CVertexDeclManager::create() {

  // Create & register each vertex declr
  // ..

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


