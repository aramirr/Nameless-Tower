#pragma once

#include "components/comp_base.h"
#include "geometry/transform.h"
#include "entity/common_msgs.h"

class TCompDoor: public TCompBase {

  float       radius;
  CHandle     h_entity;
  CHandle	  h_parent;
  float       speed;
  VEC3		  center;
  

  DECL_SIBLING_ACCESS();

public:
    bool  is_open;
    bool  is_closed;
	bool  left;
	bool  up;
	float radiants;
	float height;
	float current_height;
	float current_radiants;
    bool opening;
    bool closing;
  void debugInMenu();
  void load(const json& j, TEntityParseContext& ctx);
  void update(float DT);
  void destroy();
  void setEntity(CHandle new_entity);
  void open(const TMsgOpenDoor& msg);
  void close(const TMsgCloseDoor& msg);
  static void registerMsgs();
};