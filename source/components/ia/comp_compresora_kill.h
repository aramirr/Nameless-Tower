#pragma once

#include "../comp_base.h"
#include "geometry/transform.h"


class TCompCompresoraKill : public TCompBase {
  DECL_SIBLING_ACCESS();

  std::string other_name;
  bool player_in = false;

  void checkIfTouchingPlayer(const TMsgCheckPlayerIn& msg);
  void playerIn(const TMsgPlayerIn& msg);
  void playerOut(const TMsgPlayerOut& msg);

public:
  static void registerMsgs();

  void debugInMenu();
  void load(const json& j, TEntityParseContext& ctx);
  void update(float dt);

};