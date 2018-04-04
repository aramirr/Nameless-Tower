#ifndef INC_DRAW_UTILS_H_
#define INC_DRAW_UTILS_H_

#include <d3dx9.h>

class CCamera;

extern LPDIRECT3DDEVICE9 g_pd3dDevice;

// A structure for our custom vertex type
struct CUSTOMVERTEX
{
    FLOAT x, y, z;      // The untransformed, 3D position for the vertex
    DWORD color;        // The vertex color

    void assign( FLOAT ax, FLOAT ay, FLOAT az, DWORD acolor ) { x = ax; y = ay; z = az; color = acolor; }
};

// Our custom FVF, which describes our custom vertex structure
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)

void drawLine( const D3DXVECTOR3 &p0, const D3DXVECTOR3 &p1, unsigned color );
void drawBuilding( const D3DXVECTOR3 &p0, const D3DXVECTOR3 &p1, unsigned color );
void drawCube( const D3DXVECTOR3 &p0, const D3DXVECTOR3 &p1, unsigned color, unsigned color2 = 0 );
void drawCubeWired( const D3DXVECTOR3 &p0, const D3DXVECTOR3 &p1, unsigned color );

void drawAxis( float size = 1.0f );
void drawVector( float size = 1.0f, unsigned color = D3DCOLOR_RGBA( 255, 255, 255, 255 ) );
void drawCross( D3DXVECTOR3, float size=1.0f );
void drawGrid( int nsteps, float step_size );
void activateCamera( const CCamera &cam );
void drawLocalFov( float size, float angle );

bool initD3D( HWND hWnd );
void deinitD3D( );

#endif