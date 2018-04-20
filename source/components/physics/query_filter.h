#pragma once
using namespace physx;

class BasicQueryFilterCallback : public PxQueryFilterCallback {

	virtual PxQueryHitType::Enum preFilter(const PxFilterData& filterData, const PxShape* shape,
		const PxRigidActor* actor, PxHitFlags& queryFlags) override;

	virtual PxQueryHitType::Enum postFilter(const PxFilterData& filterData, const PxQueryHit& hit) override;

};
