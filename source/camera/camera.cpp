#include "mcv_platform.h"
#include "camera.h"

CCamera::CCamera() {
  aspect_ratio = 1.f;
  setPerspective(deg2rad(60.f), 1.0f, 1000.f);
  lookAt(VEC3(1, 0, 0), VEC3(0, 0, 0), VEC3(0, 1, 0));
}

void CCamera::updateViewProj() {
  view_proj = view * proj;
}

// 
void CCamera::lookAt(VEC3 new_pos, VEC3 new_target, VEC3 new_up_aux) {
  
  pos = new_pos;
  target = new_target;
  up_aux = new_up_aux;

  front = (target - pos);
  front.Normalize();

  left = new_up_aux.Cross(front);
  left.Normalize();

  up = front.Cross(left);

  view = MAT44::CreateLookAt(pos, target, up_aux);
  updateViewProj();
}

void CCamera::setPerspective(float new_fov_vertical, float new_z_near, float new_z_far) {
  fov_vertical = new_fov_vertical;
  z_near = new_z_near;
  z_far = new_z_far;
  assert(z_far > z_near);
  proj = MAT44::CreatePerspectiveFieldOfView(new_fov_vertical, (float)Render.width / (float)Render.height, new_z_near, new_z_far);
  updateViewProj();
}

