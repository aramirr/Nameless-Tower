#include "mcv_platform.h"
#include "comp_skel_lookat.h"
#include "components/comp_transform.h"
#include "skeleton/comp_skeleton.h"
#include "cal3d/cal3d.h"
#include "cal3d2engine.h"
#include "entity/entity_parser.h"

DECL_OBJ_MANAGER("skel_lookat", TCompSkelLookAt);

void TCompSkelLookAt::TCorrection::load(const json& j) {
  bone_name = j.value("bone", "");
  bone_id = -1;
  axis = loadVEC3(j["axis"]);
  local_axis_to_correct = loadVEC3(j["local_axis_to_correct"]);
  angle = deg2rad( j.value("angle", 0.f) );
  rotation_amount = j.value("rotation_amount", 0.f);
}

void TCompSkelLookAt::TCorrection::debugInMenu() {
  float angle_in_deg = rad2deg(angle);
  if (ImGui::DragFloat("Angle", &angle_in_deg, 0.5f, -180.f, 180.f))
    angle = deg2rad(angle_in_deg);
}

void TCompSkelLookAt::TCorrection::renderDebug() {

}

// -----------------------------------------
CalQuaternion findRotationFromDirToDir(CalVector a, CalVector b) {
  // Dot = cos(angle)
  float cos_angle = a * b;

  // Find rotation axis doing a cross product from a to b
  CalVector axis;

  return CalQuaternion();
}


void TCompSkelLookAt::TCorrection::apply(TCompSkeleton* c_skel, VEC3 dx_world_target) {

  CalVector world_target = DX2Cal(dx_world_target);

  // The cal3d skeleton instance
  CalSkeleton* skel = c_skel->model->getSkeleton();

  // We want to correct a bone by name, but we need a bone_id
  // to access that bone
  if (bone_id == -1)
    bone_id = skel->getCoreSkeleton()->getCoreBoneId(bone_name);
  if (bone_id == -1)   // The bone_name does not exists in the skel...
    return;

  CalBone* bone = skel->getBone(bone_id);
  assert(bone);

  // Where is the bone now in abs coords
  CalVector bone_abs_pos = bone->getTranslationAbsolute();

  // In world coordinate system...
  CalVector abs_dir = world_target - bone_abs_pos;

  // getRotationAbsolute goes from local to abs, so I need the inverse
  CalQuaternion rot_abs_to_local = bone->getRotationAbsolute();
  rot_abs_to_local.invert();

  // Convert from absolute coords to local bone coords
  CalVector local_dir = abs_dir;
  local_dir *= rot_abs_to_local;

  // Find a quaternion to rotate 'local_axis_to_correct' to 'local_dir'
  CalQuaternion q_rot = findRotationFromDirToDir(local_axis_to_correct, local_dir);

  //
  CalQuaternion rot = bone->getRotation();
  //QUAT dx_rot_extra = QUAT::CreateFromAxisAngle(axis, angle);
  CalQuaternion new_rot = q_rot;
  new_rot *= rot;

  bone->setRotation(new_rot);
  bone->calculateState();
}

void TCompSkelLookAt::load(const json& j, TEntityParseContext& ctx) {
  correction.load(j["correction"]);
}

void TCompSkelLookAt::update(float dt) {
  TCompSkeleton* c_skel = h_skeleton;

  if (c_skel == nullptr) {
    // Search the parent entity by name
    CEntity* e_entity = CHandle(this).getOwner();
    if (!e_entity)
      return;
    // The entity I'm tracking should have an skeleton component.
    h_skeleton = e_entity->get<TCompSkeleton>();
    assert(h_skeleton.isValid());
    c_skel = h_skeleton;
  }

  correction.apply(c_skel);
}

void TCompSkelLookAt::debugInMenu() {
  correction.debugInMenu();
}

void TCompSkelLookAt::renderDebug() {
  correction.renderDebug();
}