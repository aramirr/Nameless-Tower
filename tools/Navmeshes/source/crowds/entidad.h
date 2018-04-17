#ifndef INC_ENTIDAD_H_
#define INC_ENTIDAD_H_

#include <d3dx9.h>

//-----------------------------------------------------------------------------
class CEntidad3D {
	// 
	D3DXVECTOR3 position;
	float       yaw;
	D3DXMATRIX  world;
	D3DXVECTOR3 front;
	D3DXVECTOR3 left;

public:
	CEntidad3D( );
	const D3DXMATRIX &getWorld() const { return world; }
	const D3DXVECTOR3 &getPosition() const { return position; }
	const D3DXVECTOR3 &getFront() const { return front; }
	const D3DXVECTOR3 &getLeft() const { return left; }
	float getYaw() const { return yaw; }
	bool isInFront( const D3DXVECTOR3 &loc ) const;
	bool isInLeft( const D3DXVECTOR3 &loc ) const;
	bool isInConeVision( const D3DXVECTOR3 &loc, float max_solid_angle ) const;
	float getRelativeYaw( const D3DXVECTOR3 &loc ) const;
	// Distancia en coordenadas locales segun el eje front
	float getLocalZ( const D3DXVECTOR3 &loc ) const;
	// Distancia en coordenadas locales segun el eje front
	float getLocalX( const D3DXVECTOR3 &loc ) const;
	void setLocation( const D3DXVECTOR3 &new_position, float new_yaw );
	void draw(float size = 1.0f, unsigned color = 0xffffffff );
};

#endif
