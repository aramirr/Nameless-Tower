#include "mcv_platform.h"
#include "module_physics.h"
#include "entity/entity.h"
#include "components/juan/comp_transform.h"


#pragma comment(lib,"PhysX3_x64.lib")
#pragma comment(lib,"PhysX3Common_x64.lib")
#pragma comment(lib,"PhysX3Extensions.lib")
#pragma comment(lib,"PxFoundation_x64.lib")
#pragma comment(lib,"PxPvdSDK_x64.lib")
#pragma comment(lib, "PhysX3CharacterKinematic_x64.lib")

using namespace physx;


CModulePhysics::FilterGroup CModulePhysics::getFilterByName(const std::string& name)
{
    if ( strcmp("player", name.c_str()) == 0 ) {
        return CModulePhysics::FilterGroup::Player;
    }
    else if ( strcmp("enemy", name.c_str()) == 0 ) {
        return CModulePhysics::FilterGroup::Enemy;
    }
    else if ( strcmp("characters", name.c_str()) == 0 ) {
        return CModulePhysics::FilterGroup::Characters;
    }
    else if ( strcmp("wall", name.c_str()) == 0 ) {
        return CModulePhysics::FilterGroup::Wall;
    }
    else if ( strcmp("floor", name.c_str()) == 0 ) {
        return CModulePhysics::FilterGroup::Floor;
    }
    else if ( strcmp("scenario", name.c_str()) == 0 ) {
        return CModulePhysics::FilterGroup::Scenario;
    }
    return CModulePhysics::FilterGroup::All;
}

void CModulePhysics::createActor(TCompCollider& comp_collider)
{
  const TCompCollider::TConfig & config = comp_collider.config;
  CHandle h_comp_collider(&comp_collider);
  CEntity* e = h_comp_collider.getOwner();
  TCompTransform * compTransform = e->get<TCompTransform>();
  VEC3 pos = compTransform->getPosition();
  QUAT quat = compTransform->getRotation();

  PxTransform initialTrans(PxVec3(pos.x, pos.y, pos.z), PxQuat(quat.x, quat.y, quat.z, quat.w));

  PxRigidActor* actor = nullptr;
  if (config.shapeType == physx::PxGeometryType::ePLANE)
  {
    PxRigidStatic* plane = PxCreatePlane(*gPhysics, PxPlane(0, 1, 0, 0), *gMaterial);
    actor = plane;
    setupFiltering(actor, config.group, config.mask);
    gScene->addActor(*actor);
  }
  else if (config.shapeType == physx::PxGeometryType::eCAPSULE
    && config.is_character_controller)
  {
    PxControllerDesc* cDesc;
    PxCapsuleControllerDesc capsuleDesc;

    PX_ASSERT(desc.mType == PxControllerShapeType::eCAPSULE);
    capsuleDesc.height = config.height;
    capsuleDesc.radius = config.radius;
    capsuleDesc.climbingMode = PxCapsuleClimbingMode::eCONSTRAINED;
    cDesc = &capsuleDesc;

    cDesc->material = gMaterial;

    PxCapsuleController* ctrl = static_cast<PxCapsuleController*>(mControllerManager->createController(*cDesc));
    PX_ASSERT(ctrl);
    ctrl->setFootPosition(PxExtendedVec3(pos.x, pos.y, pos.z));
    actor = ctrl->getActor();
    comp_collider.controller = ctrl;
    setupFiltering(actor, config.group, config.mask);
  }
  else
  {
    PxShape* shape = nullptr;
    PxTransform offset(PxVec3(config.offset.x, config.offset.y, config.offset.z));
    if (config.shapeType == physx::PxGeometryType::eBOX)
    {
      shape = gPhysics->createShape(PxBoxGeometry(config.halfExtent.x, config.halfExtent.y, config.halfExtent.z), *gMaterial);
    }
    else if (config.shapeType == physx::PxGeometryType::eSPHERE)
    {
      shape = gPhysics->createShape(PxSphereGeometry(config.radius), *gMaterial);
      offset.p.y = config.radius;
    }
    //....todo: more shapes


    setupFiltering(shape, config.group, config.mask);
    shape->setLocalPose(offset);
   
    if (config.is_dynamic)
    {
      PxRigidDynamic* body = gPhysics->createRigidDynamic(initialTrans);
      actor = body;
      PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
    }
    else
    {
      PxRigidStatic* body = gPhysics->createRigidStatic(initialTrans);
      actor = body;
    }
    
    if (config.is_trigger)
    {
      shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
      shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
      actor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
    }
    assert(shape);
    assert(actor);
    actor->attachShape(*shape);
    shape->release();
    gScene->addActor(*actor);
  }

  comp_collider.actor = actor;
  actor->userData = h_comp_collider.asVoidPtr();

}

void CModulePhysics::setupFiltering(PxShape* shape, PxU32 filterGroup, PxU32 filterMask)
{
    PxFilterData filterData;
    filterData.word0 = filterGroup; // word0 = own ID
    filterData.word1 = filterMask;	// word1 = ID mask to filter pairs that trigger a contact callback;
    shape->setSimulationFilterData(filterData);
    shape->setQueryFilterData(filterData);
}

void CModulePhysics::setupFiltering(PxRigidActor* actor, PxU32 filterGroup, PxU32 filterMask)
{
    PxFilterData filterData;
    filterData.word0 = filterGroup; // word0 = own ID
    filterData.word1 = filterMask;	// word1 = ID mask to filter pairs that trigger a contact callback;
    const PxU32 numShapes = actor->getNbShapes();
    std::vector<PxShape*> shapes;
    shapes.resize(numShapes);
    actor->getShapes(&shapes[0], numShapes);
    for ( PxU32 i = 0; i < numShapes; i++ )
    {
        PxShape* shape = shapes[i];
        shape->setSimulationFilterData(filterData);
        shape->setQueryFilterData(filterData);
    }
}

PxFilterFlags CustomFilterShader(
  PxFilterObjectAttributes attributes0, PxFilterData filterData0,
  PxFilterObjectAttributes attributes1, PxFilterData filterData1,
  PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize
)
{
    if ( (filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1) )
    {
		if (PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1))
		{
			pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
		}
        else {
            pairFlags = PxPairFlag::eCONTACT_DEFAULT | PxPairFlag::eNOTIFY_TOUCH_FOUND;
        }
        return PxFilterFlag::eDEFAULT;
    }
    return PxFilterFlag::eSUPPRESS;
}

bool CModulePhysics::start()
{
    gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);
    assert(gFoundation != nullptr );
    
    if ( !gFoundation ) {
        return false;
    }

    gPvd = PxCreatePvd(*gFoundation);
    if ( !gPvd ) {
        return false;
    }

    PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
    bool  is_ok = gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

    gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true, gPvd);
    if (!gPhysics)
      fatal("PxCreatePhysics failed");

    PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
    sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
    gDispatcher = PxDefaultCpuDispatcherCreate(2);
    sceneDesc.cpuDispatcher = gDispatcher;
    sceneDesc.filterShader = CustomFilterShader;
    gScene = gPhysics->createScene(sceneDesc);
    gScene->setFlag(PxSceneFlag::eENABLE_ACTIVE_ACTORS, true);
    PxPvdSceneClient* pvdClient = gScene->getScenePvdClient();
    if ( pvdClient )
    {
        pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
        pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
        pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
    }
    gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

    mControllerManager = PxCreateControllerManager(*gScene);

    gScene->setSimulationEventCallback(&customSimulationEventCallback);
    return true;
}

void CModulePhysics::update(float delta)
{
  if (!gScene)
    return;
  gScene->simulate(delta);
  gScene->fetchResults(true);

  PxU32 nbActorsOut = 0;
  PxActor**actors = gScene->getActiveActors(nbActorsOut);

  for (unsigned int i = 0; i < nbActorsOut; ++i) {
    if (actors[i]->is<PxRigidActor>())
    {
      PxRigidActor* rigidActor = ((PxRigidActor*)actors[i]);
      PxTransform PxTrans =	rigidActor->getGlobalPose();
      PxVec3 pxpos = PxTrans.p;
      PxQuat pxq = PxTrans.q;
      CHandle h_comp_collider;
      h_comp_collider.fromVoidPtr(rigidActor->userData);
      CEntity* e = h_comp_collider.getOwner();
      TCompTransform * compTransform = e->get<TCompTransform>();
      TCompCollider* compCollider= h_comp_collider;
      if (compCollider->controller)
      {
        PxExtendedVec3 pxpos_ext = compCollider->controller->getFootPosition();
        pxpos.x = pxpos_ext.x;
        pxpos.y = pxpos_ext.y;
        pxpos.z = pxpos_ext.z;
      }
      else 
      {
        compTransform->setRotation(QUAT(pxq.x, pxq.y, pxq.z, pxq.w));
      }
      compTransform->setPosition(VEC3(pxpos.x, pxpos.y, pxpos.z));
    }
  }
}

void CModulePhysics::render()
{
}

void CModulePhysics::CustomSimulationEventCallback::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
  for (PxU32 i = 0; i < count; ++i)
  {
    if (pairs[i].flags & (PxTriggerPairFlag::eREMOVED_SHAPE_TRIGGER | PxTriggerPairFlag::eREMOVED_SHAPE_OTHER
 ))
    {
      continue;
    }
    CHandle h_trigger_comp_collider;
    h_trigger_comp_collider.fromVoidPtr( pairs[i].triggerActor->userData);
    
    CHandle h_other_comp_collider;
    h_other_comp_collider.fromVoidPtr(pairs[i].otherActor->userData);
		CEntity* e_trigger = h_trigger_comp_collider.getOwner();
		CEntity* e_other = h_other_comp_collider.getOwner();

    if (pairs[i].status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
    {
			e_trigger->sendMsg(TMsgTriggerEnter{ h_other_comp_collider.getOwner() });
    }
    else if (pairs[i].status == PxPairFlag::eNOTIFY_TOUCH_LOST)
    {
      e_trigger->sendMsg(TMsgTriggerExit{ h_other_comp_collider.getOwner() });
    }
  }
}

/*void CModulePhysics::CustomSimulationEventCallback::onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs) {
	for (PxU32 i = 0; i < nbPairs; ++i)
	{
		if (pairs[i].flags & (PxContactPairFlag::eREMOVED_SHAPE_0 | PxContactPairFlag::eREMOVED_SHAPE_1))
		{
			continue;
		}
		CHandle h_collider_comp_collider;
		h_collider_comp_collider.fromVoidPtr(pairs[i].shapes[0]->userData);

		CHandle h_other_comp_collider;
		h_other_comp_collider.fromVoidPtr(pairs[i].shapes[1]->userData);
		CEntity* e_collider = h_collider_comp_collider.getOwner();
		CEntity* e_other = h_other_comp_collider.getOwner();
		
		//e_collider->sendMsg(TMsgColliderEnter{ "Hello" });

	}
}*/