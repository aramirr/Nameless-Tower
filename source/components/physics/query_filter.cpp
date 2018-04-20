#include "mcv_platform.h"
#include "query_filter.h"
#include "modules\system\module_physics.h"

using namespace physx;

PxQueryHitType::Enum BasicQueryFilterCallback::preFilter(const PxFilterData& filter_data_a, const PxShape* shape, const PxRigidActor* actor, PxHitFlags& queryFlags) {
	CModulePhysics::FilterGroup::Player;
	CModulePhysics::FilterGroup::Wall;
	PxFilterData& filter_data_b = shape->getQueryFilterData();
	if ((filter_data_a.word0 & filter_data_b.word1) && (filter_data_b.word0 & filter_data_a.word1)) {
		return PxQueryHitType::eBLOCK;
	}
	return PxQueryHitType::eNONE;
}

PxQueryHitType::Enum BasicQueryFilterCallback::postFilter(const PxFilterData& filterData, const PxQueryHit& hit) {
	return PxQueryHitType::eNONE;
}
