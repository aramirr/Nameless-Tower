#ifndef INC_CAMERA_H_
#define INC_CAMERA_H_

#include <d3dx9.h>

class CCamera {
public:
	// View
	D3DXVECTOR3 position;
	D3DXVECTOR3 target;
	D3DXVECTOR3 up_aux_axis;			// 0,1,0

	// Main axis
	D3DXVECTOR3 front;
	D3DXVECTOR3 left;
	D3DXVECTOR3 up;

	float       yaw, pitch; //, roll;

	// Projection
	float       fov_vertical_in_radians;	
	float       aspect_ratio;				// De la ventana
	float       znear, zfar;

	// Matrixs
	D3DXMATRIX  view;
	D3DXMATRIX  projection;
	D3DXMATRIX  view_projection;

	void updateViewProjection();

public:
	CCamera();
	const D3DXVECTOR3 &getPosition() const { return position; }
	const D3DXVECTOR3 &getTarget() const { return target; }
	const D3DXVECTOR3 &getUpAuxAxis() const { return up_aux_axis; }

	const D3DXMATRIX &getView() const { return view; }
	const D3DXMATRIX &getProjection() const { return projection; }

	float getYaw() const { return yaw; }
	float getPitch() const { return pitch; }

	void setProjectionParams( float new_fov_vertical
		                    , float new_aspect_ratio
							, float new_znear
							, float new_zfar
							);

	void lookAt( const D3DXVECTOR3 &new_position
	            , const D3DXVECTOR3 &new_target
	            , const D3DXVECTOR3 &new_up_aux
				);

  D3DXVECTOR3 getRayDirection( float normalized_x_from_left_to_right, float normalized_y_from_top_to_bottom ) const;
  D3DXVECTOR3 getHitXZOnGround( float normalized_x_from_left_to_right, float normalized_y_from_top_to_bottom, float height ) const;
};

#endif
