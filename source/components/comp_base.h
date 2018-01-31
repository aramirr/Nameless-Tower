#ifndef INC_COMP_BASE_H_
#define INC_COMP_BASE_H_

struct TEntityParseContext;

struct TCompBase {
  void debugInMenu() {}
  void load( const json& j, TEntityParseContext& ctx ) {}
  void update(float dt) {}

  static void registerMsgs() {}
};

#define DECL_SIBLING_ACCESS()   \
  template< typename TComp >    \
  CHandle get() {                            \
    CEntity* e = CHandle(this).getOwner();   \
    if (!e)                                  \
      return CHandle();                      \
    return e->get<TComp>();                  \
  }


#endif
