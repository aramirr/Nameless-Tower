#include "draw_utils.h"
#include "camera.h"
#include "angular.h"

LPDIRECT3D9             g_pD3D = NULL; // Used to create the D3DDevice
LPDIRECT3DDEVICE9       g_pd3dDevice = NULL; // Our rendering device

//-----------------------------------------------------------------------------
bool initD3D( HWND hWnd )
{
    // Create the D3D object.
    if( NULL == ( g_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
        return false;

    // Set up the structure used to create the D3DDevice
    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory( &d3dpp, sizeof( d3dpp ) );
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

    // Create the D3DDevice
    if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                                      D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                                      &d3dpp, &g_pd3dDevice ) ) )
    {
        return false;
    }

    // Turn off culling, so we see the front and back of the triangle
    g_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

    // Turn off D3D lighting, since we are providing our own vertex colors
    g_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

    // Turn on the zbuffer
    g_pd3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );

    return true;
}

void deinitD3D( ) {
    if( g_pd3dDevice != NULL )
        g_pd3dDevice->Release();

    if( g_pD3D != NULL )
        g_pD3D->Release();
}

//-----------------------------------------------------------------------------
void drawLine( const D3DXVECTOR3 &p0, const D3DXVECTOR3 &p1, unsigned color ) {
	CUSTOMVERTEX data[2] = { 
	  { p0.x, p0.y, p0.z, color }
	, { p1.x, p1.y, p1.z, color }
	};
	g_pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
	g_pd3dDevice->DrawPrimitiveUP( D3DPT_LINELIST, 1, data, sizeof( CUSTOMVERTEX ) );
}

void drawAxis( float size ) {
	drawLine( D3DXVECTOR3(0,0,0),D3DXVECTOR3(1*size,0,0), D3DCOLOR_RGBA( 255,0,0,255) );
	drawLine( D3DXVECTOR3(0,0,0),D3DXVECTOR3(0,2*size,0), D3DCOLOR_RGBA( 0,255,0,255) );
	drawLine( D3DXVECTOR3(0,0,0),D3DXVECTOR3(0,0,3*size), D3DCOLOR_RGBA( 0,0,255,255) );
}

void drawLocalFov( float size, float angle ) {
	drawLine( D3DXVECTOR3(0,0,0),size*yawToAxis( angle * 0.5f ), D3DCOLOR_RGBA( 255,0,0,255) );
	drawLine( D3DXVECTOR3(0,0,0),size*yawToAxis( -angle * 0.5f ), D3DCOLOR_RGBA( 255,0,0,255) );
}

//-----------------------------------------------------------------------------
void drawGrid( int nsteps, float step_size ) {
	unsigned color1 = D3DCOLOR_RGBA( 80,80,80,255);
	unsigned color2 = D3DCOLOR_RGBA( 120,120,120,255);
	for( int i=-nsteps; i<=nsteps; ++i ) {
		unsigned color = ( i % 5 == 0 ) ? color2 : color1;
		drawLine( D3DXVECTOR3( i * step_size, 0.0f, nsteps * step_size )
			    , D3DXVECTOR3( i * step_size, 0.0f,-nsteps * step_size )
				, color );
		drawLine( D3DXVECTOR3( nsteps * step_size, 0.0f, i * step_size )
			    , D3DXVECTOR3(-nsteps * step_size, 0.0f, i * step_size )
				, color );
	}
}

void activateCamera( const CCamera &cam ) {
    g_pd3dDevice->SetTransform( D3DTS_VIEW, &cam.getView() );
    g_pd3dDevice->SetTransform( D3DTS_PROJECTION, &cam.getProjection() );
	D3DXMATRIX world;
	D3DXMatrixIdentity( &world );
    g_pd3dDevice->SetTransform( D3DTS_WORLD, &world );
}


void drawCross( D3DXVECTOR3 pos, float size ) 
{
	drawLine( D3DXVECTOR3(pos.x-size,pos.y,pos.z),D3DXVECTOR3(pos.x+size,pos.y,pos.z), D3DCOLOR_RGBA( 255,0,0,255) );
	drawLine( D3DXVECTOR3(pos.x,pos.y-size,pos.z),D3DXVECTOR3(pos.x,pos.y+size,pos.z), D3DCOLOR_RGBA( 255,0,0,255) );
	drawLine( D3DXVECTOR3(pos.x,pos.y,pos.z-size),D3DXVECTOR3(pos.x,pos.y,pos.z+size), D3DCOLOR_RGBA( 255,0,0,255) );
}


void drawVector(float size, unsigned color ) {
  drawLine( D3DXVECTOR3( 0, 0, 0 ), D3DXVECTOR3( -1 * size, 0, -0.5*size ), color );
  drawLine( D3DXVECTOR3( 0, 0, 0 ), D3DXVECTOR3( 1 * size, 0, -0.5* size ), color );
  drawLine( D3DXVECTOR3( 0, 0, 2 * size ), D3DXVECTOR3( -1 * size, 0, -0.5 * size ), color );
  drawLine( D3DXVECTOR3( 0, 0, 2 * size ), D3DXVECTOR3( 1 * size, 0, -0.5 * size ), color );
}

void drawBuilding(const D3DXVECTOR3 &p0, const D3DXVECTOR3 &p1, unsigned color)
{
	CUSTOMVERTEX top[6] = {
			{ p0.x, p1.y, p0.z, color }
			, { p0.x, p1.y, p1.z, color }
			, { p1.x, p1.y, p0.z, color }
			, { p0.x, p1.y, p1.z, color }
			, { p1.x, p1.y, p0.z, color }
			, { p1.x, p1.y, p1.z, color }

	};

	g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
	g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, top, sizeof(CUSTOMVERTEX));

	color = D3DCOLOR_RGBA(192, 0, 0, 255);

	CUSTOMVERTEX xmin[6] = {
			{p0.x, p0.y, p0.z, color }
			, { p0.x, p0.y, p1.z, color }
			, { p0.x, p1.y, p0.z, color }
			, { p0.x, p0.y, p1.z, color }
			, { p0.x, p1.y, p0.z, color }
			, { p0.x, p1.y, p1.z, color }

	};
	g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
	g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, xmin, sizeof(CUSTOMVERTEX));

	color = D3DCOLOR_RGBA(128, 0, 0, 255);

	CUSTOMVERTEX xmax[6] = {
		 { p1.x, p0.y, p1.z, color }
		 , { p1.x, p1.y, p0.z, color }
			,{ p1.x, p0.y, p0.z, color }
			, { p1.x, p1.y, p1.z, color }
			, { p1.x, p1.y, p0.z, color }
			, { p1.x, p0.y, p1.z, color }

	};
	g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
	g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, xmax, sizeof(CUSTOMVERTEX));


	color = D3DCOLOR_RGBA(160, 0, 0, 255);

	CUSTOMVERTEX zmin[6] = {
			{ p0.x, p0.y, p0.z, color }
			, { p0.x, p1.y, p0.z, color }
			, { p1.x, p0.y, p0.z, color }
			, { p0.x, p1.y, p0.z, color }
			, { p1.x, p0.y, p0.z, color }
			, { p1.x, p1.y, p0.z, color }

	};
	g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
	g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, zmin, sizeof(CUSTOMVERTEX));


	color = D3DCOLOR_RGBA(210, 0, 0, 255);

	CUSTOMVERTEX zmax[6] = {
			 { p1.x, p0.y, p1.z, color }
			, { p0.x, p1.y, p1.z, color }
			, { p0.x, p0.y, p1.z, color }
			, { p1.x, p1.y, p1.z, color }
			, { p1.x, p0.y, p1.z, color }
			, { p0.x, p1.y, p1.z, color }

	};
	g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
	g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, zmax, sizeof(CUSTOMVERTEX));

}

void drawCube( const D3DXVECTOR3 &p0, const D3DXVECTOR3 &p1, unsigned color, unsigned color2 ) {
  if( color2 == 0 )
    color2 = color;

  CUSTOMVERTEX top[ 6 ] = {
      { p0.x, p1.y, p0.z, color }
      , { p0.x, p1.y, p1.z, color }
      , { p1.x, p1.y, p0.z, color }
      , { p0.x, p1.y, p1.z, color }
      , { p1.x, p1.y, p0.z, color }
      , { p1.x, p1.y, p1.z, color }

  };

  g_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

  g_pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
  g_pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLELIST, 2, top, sizeof( CUSTOMVERTEX ) );

  CUSTOMVERTEX xmin[ 6 ] = {
      { p0.x, p0.y, p0.z, color2 }
      , { p0.x, p0.y, p1.z, color2 }
      , { p0.x, p1.y, p0.z, color2 }
      , { p0.x, p0.y, p1.z, color2 }
      , { p0.x, p1.y, p0.z, color2 }
      , { p0.x, p1.y, p1.z, color2 }

  };
  g_pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
  g_pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLELIST, 2, xmin, sizeof( CUSTOMVERTEX ) );

  CUSTOMVERTEX xmax[ 6 ] = {
      { p1.x, p0.y, p1.z, color2 }
      , { p1.x, p1.y, p0.z, color2 }
      , { p1.x, p0.y, p0.z, color2 }
      , { p1.x, p1.y, p1.z, color2 }
      , { p1.x, p1.y, p0.z, color2 }
      , { p1.x, p0.y, p1.z, color2 }

  };
  g_pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
  g_pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLELIST, 2, xmax, sizeof( CUSTOMVERTEX ) );


  CUSTOMVERTEX zmin[ 6 ] = {
      { p0.x, p0.y, p0.z, color2 }
      , { p0.x, p1.y, p0.z, color2 }
      , { p1.x, p0.y, p0.z, color2 }
      , { p0.x, p1.y, p0.z, color2 }
      , { p1.x, p0.y, p0.z, color2 }
      , { p1.x, p1.y, p0.z, color2 }

  };
  g_pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
  g_pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLELIST, 2, zmin, sizeof( CUSTOMVERTEX ) );


  CUSTOMVERTEX zmax[ 6 ] = {
      { p1.x, p0.y, p1.z, color2 }
      , { p0.x, p1.y, p1.z, color2 }
      , { p0.x, p0.y, p1.z, color2 }
      , { p1.x, p1.y, p1.z, color2 }
      , { p1.x, p0.y, p1.z, color2 }
      , { p0.x, p1.y, p1.z, color2 }

  };
  g_pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
  g_pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLELIST, 2, zmax, sizeof( CUSTOMVERTEX ) );

}

void drawCubeWired( const D3DXVECTOR3 &p0, const D3DXVECTOR3 &p1, unsigned color ) {
  // back
  drawLine( D3DXVECTOR3( p0.x, p0.y, p0.z ), D3DXVECTOR3( p1.x, p0.y, p0.z ), color );    // bottom
  drawLine( D3DXVECTOR3( p0.x, p0.y, p0.z ), D3DXVECTOR3( p0.x, p1.y, p0.z ), color );    // left
  drawLine( D3DXVECTOR3( p0.x, p1.y, p0.z ), D3DXVECTOR3( p1.x, p1.y, p0.z ), color );    // top
  drawLine( D3DXVECTOR3( p1.x, p1.y, p0.z ), D3DXVECTOR3( p1.x, p0.y, p0.z ), color );    // right

  // middle
  drawLine( D3DXVECTOR3( p0.x, p0.y, p0.z ), D3DXVECTOR3( p0.x, p0.y, p1.z ), color );    // bottom-left
  drawLine( D3DXVECTOR3( p1.x, p0.y, p0.z ), D3DXVECTOR3( p1.x, p0.y, p1.z ), color );    // bottom-right
  drawLine( D3DXVECTOR3( p0.x, p1.y, p0.z ), D3DXVECTOR3( p0.x, p1.y, p1.z ), color );    // top-left
  drawLine( D3DXVECTOR3( p1.x, p1.y, p0.z ), D3DXVECTOR3( p1.x, p1.y, p1.z ), color );    // top-right

  // front
  drawLine( D3DXVECTOR3( p0.x, p0.y, p1.z ), D3DXVECTOR3( p1.x, p0.y, p1.z ), color );    // bottom
  drawLine( D3DXVECTOR3( p0.x, p0.y, p1.z ), D3DXVECTOR3( p0.x, p1.y, p1.z ), color );    // left
  drawLine( D3DXVECTOR3( p0.x, p1.y, p1.z ), D3DXVECTOR3( p1.x, p1.y, p1.z ), color );    // top
  drawLine( D3DXVECTOR3( p1.x, p1.y, p1.z ), D3DXVECTOR3( p1.x, p0.y, p1.z ), color );    // right
}
