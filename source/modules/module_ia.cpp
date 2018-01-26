#include "mcv_platform.h"
#include "module_ia.h"


void CModuleIA::update(float delta)
{
  // For each controller, do recalc

}

void CModuleIA::render()
{

}

// 
IAIController* CModuleIA::getNewAIControler(const json& j, TEntity* e) {
  IAIController* c = nullptr;
  
  if (j["type"] == "turret") {
    dbg("Creating a turret IA controller...\n");
    // c = new CAITurret;
    // c->configure( j );
    // c->setEntity( entity );

  }
  // else if( ... ){
  // ...
  // }
  
  //if( c )
  //  controllers.push_back( c );

  return c;
}