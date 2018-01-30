#ifndef INC_COMP_BASE_H_
#define INC_COMP_BASE_H_

struct TEntityParseContext;

struct TCompBase {
  void debugInMenu() {}
  void load( const json& j, TEntityParseContext& ctx ) {}
  void update(float dt) {}

  static void registerMsgs() {}
};

#endif
