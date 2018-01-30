#pragma once

#include "comp_base.h"

class TCompName : public TCompBase {
  static const size_t max_size = 64;
  char name[max_size];
public:

  const char* getName() const { return name; }
  void setName(const char* new_name) {
    assert(strlen(new_name) < max_size);
    strcpy(name, new_name);
  }

  void debugInMenu();
  void load(const json& j, TEntityParseContext& ctx);
};