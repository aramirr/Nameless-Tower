#pragma once

// Forward declarations
class CRenderMesh;
class CTexture;
class CRenderTechnique;
class IAIController;

struct TEntity {
  std::string  name;
  CTransform   transform;
  const CRenderMesh* mesh = nullptr;
  const CTexture* texture = nullptr;
  CRenderTechnique* tech = nullptr;
  IAIController* ai_controller = nullptr;
  
  void load(const json& j);
};

