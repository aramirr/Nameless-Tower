#pragma once

// Forward declarations
class CRenderMesh;
class CRenderTechnique;
class IAIController;

struct TEntity {
  std::string  name;
  CTransform   transform;
  CRenderMesh* mesh = nullptr;
  CRenderTechnique* tech = nullptr;
  IAIController* ai_controller = nullptr;
  
  void load(const json& j);
};

