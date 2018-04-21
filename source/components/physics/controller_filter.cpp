#include "mcv_platform.h"
#include "controller_filter.h"
#include "components\physics\comp_collider.h"


bool ControllerFilterCallback::filter(const PxController& a, const PxController& b) {
	PxShape* shape_a;
	a.getActor()->getShapes(&shape_a, 1);
	PxFilterData filter_data_a = shape_a->getSimulationFilterData();

	PxShape* shape_b;
	b.getActor()->getShapes(&shape_b, 1);
	PxFilterData filter_data_b = shape_b->getSimulationFilterData();

	if ((filter_data_a.word0 & filter_data_b.word1) && (filter_data_b.word0 & filter_data_a.word1)) {
		return true;
	}
	return false;

}

PxQueryHitType::Enum ControllerFilterCallback::preFilter(const PxFilterData& filter_data_a, const PxShape* shape, const PxRigidActor* actor, PxHitFlags& queryFlags) {
	const PxFilterData& filter_data_b = shape->getSimulationFilterData();	
	if ((filter_data_a.word0 & filter_data_b.word1) && (filter_data_b.word0 & filter_data_a.word1)) {		
		if ((filter_data_a.word0 == CModulePhysics::FilterGroup::Projectile) && (filter_data_b.word0 == CModulePhysics::FilterGroup::Player) || (filter_data_b.word0 == CModulePhysics::FilterGroup::Projectile) && (filter_data_a.word0 == CModulePhysics::FilterGroup::Player)) {
			// Mandar mensajes de muerte al player
		}
		return PxQueryHitType::eBLOCK;
	}
	return PxQueryHitType::eNONE;
}

PxQueryHitType::Enum ControllerFilterCallback::postFilter(const PxFilterData& filterData, const PxQueryHit & hit) {
	return PxQueryHitType::eBLOCK;
}
