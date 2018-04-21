#pragma once

#include "modules/module.h"
#include "components/physics/comp_collider.h"
#include "PxPhysicsAPI.h"

class CModulePhysics : public IModule
{
public:

    enum FilterGroup {
        Wall = 1 << 0,
        Floor = 1 << 1,
        Player = 1 << 2,
        Enemy = 1 << 3,
				Windstrike = 1 << 4,
        Scenario = Wall | Floor,
        Characters = Player | Enemy,
				AllUnlessPlayer = -Player,
				PlayerMask = Scenario,
        All = -1
    };

  CModulePhysics(const std::string& aname) : IModule(aname) { }
  virtual bool start() override;
  virtual void update(float delta) override;
  virtual void render() override;
  FilterGroup getFilterByName(const std::string& name);
  void createActor(TCompCollider& comp_collider);
  void setupFiltering(physx::PxShape* shape, physx::PxU32 filterGroup, physx::PxU32 filterMask);
  void setupFiltering(physx::PxRigidActor* actor, physx::PxU32 filterGroup, physx::PxU32 filterMask);

private:
    physx::PxDefaultAllocator gDefaultAllocatorCallback;
    physx::PxDefaultErrorCallback gDefaultErrorCallback;

    physx::PxPhysics*				gPhysics;

    physx::PxDefaultCpuDispatcher*	gDispatcher;
    physx::PxScene*				gScene;

    physx::PxMaterial*				gMaterial;

    physx::PxPvd*                  gPvd;
    physx::PxFoundation*			gFoundation;
    physx::PxControllerManager*     mControllerManager;
    
  class CustomSimulationEventCallback : public physx::PxSimulationEventCallback
  {
    virtual void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count) override {};
    virtual void onWake(physx::PxActor** actors, physx::PxU32 count) override {};
    virtual void onSleep(physx::PxActor** actors, physx::PxU32 count) override {};
	virtual void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs) override {};
    virtual void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count);
    virtual void onAdvance(const physx::PxRigidBody*const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count)override {};

  };

  CustomSimulationEventCallback customSimulationEventCallback;
};
