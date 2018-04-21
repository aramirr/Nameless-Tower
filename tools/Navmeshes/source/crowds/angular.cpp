#include <cassert>
#include "angular.h"

// ---------------------------------------------
float axisToYaw( const D3DXVECTOR3 &axis ) {
	return atan2( axis.x, axis.z );
}

D3DXVECTOR3 yawToAxis( float yaw ) {
	return D3DXVECTOR3( sin( yaw ), 0.0f, cos( yaw ) );
}

float wrapAngle( float yaw ) {
	float prev_yaw = yaw;
	while( yaw > deg2rad( 180 ) )
		yaw -= deg2rad(360.0f);
	while( yaw < -deg2rad( 180 ) )
		yaw += deg2rad( 360.0f);
	// normal case, already in the valid range
	assert( yaw >= deg2rad(-180.0f) && yaw <= deg2rad(180.0f) );
	return yaw;
}


// ---------------------------------------------
void axisToYawPitch( const D3DXVECTOR3 &axis, float &yaw, float &pitch ) {
	yaw = axisToYaw( axis );
	float mdo_xz = sqrtf( axis.x*axis.x + axis.z*axis.z );
	pitch = atan2( axis.y, mdo_xz );
}

D3DXVECTOR3 yawPitchToAxis( float yaw, float pitch ) {
	return D3DXVECTOR3( sin( yaw ) * cos( pitch )
		              ,              sin( pitch )
					  , cos( yaw ) * cos( pitch ) 
					  );
	
}
