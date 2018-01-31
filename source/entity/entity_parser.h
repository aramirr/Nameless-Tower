#pragma once

struct TEntityParseContext {

  TEntityParseContext* parent = nullptr;
  std::string          filename;

  VHandles             entities_loaded;
};

bool parseScene(const std::string& filename, TEntityParseContext& ctx);

