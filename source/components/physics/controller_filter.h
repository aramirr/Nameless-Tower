#pragma once
using namespace physx;

class ControllerFilterCallback : public PxControllerFilterCallback, public PxQueryFilterCallback {

public:

	bool filter(const PxController& a, const PxController& b) override;
	
	PxQueryHitType::Enum preFilter(const physx::PxFilterData& filterData, const physx::PxShape* shape, const physx::PxRigidActor* actor, physx::PxHitFlags& queryFlags);
	PxQueryHitType::Enum postFilter(const PxFilterData& filterData, const PxQueryHit& hit);
};