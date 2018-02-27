#pragma once

#include "comp_base.h"

class CRenderMesh;
class CTexture;
class CRenderTechnique;
class CMaterial;

class TCompRender : public TCompBase {
  void loadMesh(const json& j, TEntityParseContext& ctx);

public:
  VEC4 color = VEC4(1, 1, 1, 1);
  bool is_active = true;

  // This represents a single object mesh with several materials. Not multiples meshes
  const CRenderMesh* mesh = nullptr;
	std::vector<const CRenderMesh*> meshes;
  std::vector<const CMaterial*> materials;

  void debugInMenu();
  void renderDebug();
  void load(const json& j, TEntityParseContext& ctx);
  DECL_SIBLING_ACCESS();
};