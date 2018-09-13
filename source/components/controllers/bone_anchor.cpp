#include "mcv_platform.h"
#include "bone_anchor.h"
#include "components/juan/comp_transform.h"
#include "geometry/curve.h"
#include "skeleton\comp_skeleton.h"
#include "cal3d/cal3d.h"

DECL_OBJ_MANAGER("bone_anchor", TCompBoneAnchor);

void TCompBoneAnchor::debugInMenu() {
	
}

void TCompBoneAnchor::load(const json& j, TEntityParseContext& ctx) {
	std::string owner_name = j["skel_owner"];
	skeleton_owner = (CHandle)getEntityByName(owner_name);
	bone_id = j["bone_id"];
}
 
void TCompBoneAnchor::update(float DT)
{
	TCompTransform* c_transform = get<TCompTransform>();
	CHandle dummy = (CHandle)getEntityByName("The Player");
	CEntity* dummyE = dummy;
	TCompTransform* d_transform = dummyE->get<TCompTransform>();
	TCompSkeleton* d_skeleton = dummyE->get<TCompSkeleton>();
	CalModel* model = d_skeleton->model;
	CalBone* cal_bone = model->getSkeleton()->getBone(bone_id);

	VEC3 pos = VEC3(cal_bone->getTranslationAbsolute().x, cal_bone->getTranslationAbsolute().y, cal_bone->getTranslationAbsolute().z );
	c_transform->setPosition(pos);
}



