#include "camera.h"
#include "angular.h"

// ----------------------------------------------------
CCamera::CCamera() {
	D3DXMatrixIdentity( &view );
	setProjectionParams( 60 * D3DX_PI / 180.0f, 1.0f, 1.0f, 100.0f );
	lookAt( D3DXVECTOR3( 1,1,1 ), D3DXVECTOR3(0,0,0),D3DXVECTOR3(0,1,0) );
}

// ----------------------------------------------------
void CCamera::updateViewProjection() {
	view_projection = view * projection;
}

// ----------------------------------------------------
void CCamera::setProjectionParams( float new_fov_vertical_in_radians
	                    , float new_aspect_ratio
						, float new_znear
						, float new_zfar
						) {

	fov_vertical_in_radians = new_fov_vertical_in_radians;
	aspect_ratio = new_aspect_ratio;
	znear = new_znear;
	zfar = new_zfar;
	D3DXMatrixPerspectiveFovRH( &projection
		                      , fov_vertical_in_radians
							  , aspect_ratio
							  , znear
							  , zfar );
	
	updateViewProjection();
}

// ----------------------------------------------------
void CCamera::lookAt( const D3DXVECTOR3 &new_position
	            , const D3DXVECTOR3 &new_target
	            , const D3DXVECTOR3 &new_up_aux
				) {
	
	// Copy input params				
	position = new_position;
	target   = new_target;
	up_aux_axis = new_up_aux;

	// Update view matrix
    D3DXMatrixLookAtRH( &view, &position, &target, &up_aux_axis );

	// Refresh axis
	front = target - position;
	D3DXVec3Normalize( &front, &front );
	// left = up_aux_axis x front
	D3DXVec3Cross( &left, &up_aux_axis, &front );
	D3DXVec3Normalize( &left, &left );
	// Up = front x left (already orthonormal)
	D3DXVec3Cross( &up, &front, &left );

	// Refresh angles
	axisToYawPitch( front, yaw, pitch );

	updateViewProjection();
}

// (0.0, 0.0f) Upper - Left corner of the camera frustum
// (1.0, 1.0f) Lower - Right corner of the camera frustum
// (0.5, 0.5f) Center of the camera -> front
D3DXVECTOR3 CCamera::getRayDirection( float normalized_x_from_left_to_right, float normalized_y_from_top_to_bottom ) const {
  float x = -((2.0f * normalized_x_from_left_to_right) - 1.0f);
  float y = ((2.0f * normalized_y_from_top_to_bottom) - 1.0f);
  float d = 1.0f / tanf( fov_vertical_in_radians * 0.5f );

  D3DXVECTOR3 ray_dir = front * d
    + left * x * aspect_ratio
    - up * y;
  D3DXVec3Normalize( &ray_dir, &ray_dir );
  return ray_dir;
}

// r(t) = p0 + v*t   t = -p0y / vy
D3DXVECTOR3 CCamera::getHitXZOnGround( float normalized_x_from_left_to_right, float normalized_y_from_top_to_bottom, float height ) const {
  D3DXVECTOR3 ray_dir = getRayDirection( normalized_x_from_left_to_right, normalized_y_from_top_to_bottom );
  if( ray_dir.y != 0.0f ) {
    float thit = -(position.y - height) / ray_dir.y;
    return position + ray_dir * thit;
  }
  else {	// Parallel to the ground
    return D3DXVECTOR3( 1000.0f, height, 1000.0f );	// We return a very far away point
  }
}
