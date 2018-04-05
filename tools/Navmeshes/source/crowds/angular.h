#ifndef INC_ANGULAR_H_
#define INC_ANGULAR_H_

#include <cmath>
#include <d3dx9.h>

#ifndef M_PI
#define M_PI    ((float)  3.141592654f)
#endif

#define deg2rad( degree ) ((degree) * (M_PI / 180.0f))
#define rad2deg( radian ) ((radian) * (180.0f / M_PI))

float axisToYaw( const D3DXVECTOR3 &axis );
D3DXVECTOR3 yawToAxis( float yaw );

void axisToYawPitch( const D3DXVECTOR3 &axis, float &yaw, float &pitch );
D3DXVECTOR3 yawPitchToAxis( float yaw, float pitch );

float wrapAngle( float yaw );
float angleDifference( float yaw_src, float yaw_dst );

#endif
