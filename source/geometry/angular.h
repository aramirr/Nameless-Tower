#pragma once

#define deg2rad(_deg)  ( (_deg) * (float) M_PI / 180.f)
#define rad2deg(_rad)  ( (_rad) / (float) M_PI * 180.f)

VEC3 getVectorFromYaw(float yaw) {
  return VEC3(sinf(yaw), 0.f, cosf(yaw));
}

float getYawFromVector(VEC3 front) {
  return atan2f(front.x, front.z);
}

