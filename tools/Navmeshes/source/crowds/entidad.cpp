#include "entidad.h"
#include "angular.h"
#include "draw_utils.h"

//-----------------------------------------------------------------------------
CEntidad3D::CEntidad3D( ) {
	setLocation( D3DXVECTOR3( 0,0,0 ), 0.0f );
}

bool CEntidad3D::isInFront( const D3DXVECTOR3 &loc ) const {
	D3DXVECTOR3 delta = loc - position;
	return D3DXVec3Dot( &front, &delta ) > 0.0f;
}
bool CEntidad3D::isInLeft( const D3DXVECTOR3 &loc ) const {
	D3DXVECTOR3 delta = loc - position;
	return D3DXVec3Dot( &left, &delta ) > 0.0f;
}
bool CEntidad3D::isInConeVision( const D3DXVECTOR3 &loc, float max_solid_angle ) const {
	D3DXVECTOR3 delta = loc - position;
	D3DXVec3Normalize( &delta, &delta );
	// cos(alfa) > cos( fov/2)
	return D3DXVec3Dot( &front, &delta ) > cos( max_solid_angle * 0.5f );
}

float CEntidad3D::getRelativeYaw( const D3DXVECTOR3 &loc ) const {
/*	D3DXVECTOR3 delta = loc - position;
	float yaw1 = axisToYaw( delta );
	return  angleDifference( yaw, yaw1 );
*/
	D3DXVECTOR3 delta = loc - position;
	float front_amount = D3DXVec3Dot( &front, &delta );
	float left_amount = D3DXVec3Dot( &left,  &delta );
	return atan2( left_amount, front_amount );
}

// Distancia en coordenadas locales segun el eje front
float CEntidad3D::getLocalZ( const D3DXVECTOR3 &loc ) const {
	D3DXVECTOR3 delta = loc - position;
	return D3DXVec3Dot( &front, &delta );
}

// Distancia en coordenadas locales segun el eje front
float CEntidad3D::getLocalX( const D3DXVECTOR3 &loc ) const {
	D3DXVECTOR3 delta = loc - position;
	return D3DXVec3Dot( &left, &delta );
}


void CEntidad3D::setLocation( const D3DXVECTOR3 &new_position, float new_yaw ) {
	position = new_position;
	yaw      = wrapAngle( new_yaw );
	front    = yawToAxis( yaw );
	left     = yawToAxis( yaw + deg2rad( 90 ) );
	
	D3DXMATRIX rot;
	D3DXMatrixRotationY( &rot, yaw );
	D3DXMATRIX trans;
	D3DXMatrixTranslation( &trans, position.x, position.y, position.z );
	world = rot * trans;
}

void CEntidad3D::draw( float size, unsigned color ) {
	// Cambiar la world al 1,0,0
	g_pd3dDevice->SetTransform( D3DTS_WORLD, &world );
	drawVector( size, color);
}

