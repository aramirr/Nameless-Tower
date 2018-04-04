/* CROWD DYNAMICS DEMO
O: turn off  optimization by distance
G: turn off ground, so you see what's going on
S: turn off spatial index
*/

//-----------------------------------------------------------------------------
#include <Windows.h>
#include <mmsystem.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <cassert>
#pragma warning( disable : 4996 ) // disable deprecated warning 
#include <strsafe.h>
#pragma warning( default : 4996 )

#include "camera.h"
#include "angular.h"
#include "draw_utils.h"
#include "entidad.h"
#include "segment.h"
#include "agentInstance.h"
#include "spatialindex.h"
#include "ground.h"
#include "input.h"
#include "navmesh/navmesh.h"
#include "navmesh/navmesh_query.h"

extern bool isPressed( int key );
extern int window_xres, window_yres;

class CVertexBuffer {
	
	static const int separator_magic = 0x11112222;

	struct THeader {
		unsigned magic;
		unsigned version_number;
		unsigned num_faces;
		unsigned num_vertexs;
		unsigned vertex_type;
		unsigned primitive_type;
		unsigned bytes_per_vertex;
		unsigned separator;
		bool isValid() const {
			return  magic == 0x55776688 
				 && version_number == 1
				 && separator == separator_magic;
		}
	};

	LPDIRECT3DVERTEXBUFFER9 vb;
	THeader					header;
	DWORD                   flags;

public:
	bool createFromFile( const char *filename ) {

		FILE *f = fopen( filename, "rb" );
		if( !f )
			return false;
		size_t bytes_read = fread( &header, 1, sizeof( THeader ), f );
		assert( bytes_read == sizeof( THeader ) );
		if( !header.isValid() ) {
			assert( !"El fichero no es una malla valida" );
			fclose( f );
			return false;
		}


		// Assign a FVF flags based on the vertex type read from the file.
		if( header.vertex_type == 1 ) {
			flags = D3DFVF_XYZ;
			assert( header.bytes_per_vertex == 12 );
		} else if( header.vertex_type == 2 ) {
			flags = D3DFVF_XYZ|D3DFVF_TEX1;
			assert( header.bytes_per_vertex == 5*4 );
		} else {
			assert( !"Unsupported vertex type in file" );
		}

		size_t bytes_required = header.num_vertexs * header.bytes_per_vertex;

		// Create the vertex buffer.
		if( FAILED( g_pd3dDevice->CreateVertexBuffer( bytes_required,
													0, flags,
													D3DPOOL_DEFAULT, &vb, NULL ) ) ) {
			fclose( f );
			return false;
		}

		// Fill the vertex buffer. We are setting the tu and tv texture
		// coordinates, which range from 0.0 to 1.0
		void* vram_vertex;
		if( FAILED( vb->Lock( 0, 0, &vram_vertex, 0 ) ) ) {
			fclose( f );
			return false;
		}
		// Rellenar los datos del vb desde disco por ejemplo
		bytes_read = fread( vram_vertex, 1, bytes_required, f );
		assert( bytes_read == bytes_required );
		vb->Unlock();

		fclose( f );
		return true;
	}

	void renderPoints( ) {
        g_pd3dDevice->SetStreamSource( 0, vb, 0, header.bytes_per_vertex );
		g_pd3dDevice->SetFVF( flags );
        g_pd3dDevice->DrawPrimitive( D3DPT_POINTLIST, 0, header.num_vertexs );
	}
	void render( ) {
        g_pd3dDevice->SetStreamSource( 0, vb, 0, header.bytes_per_vertex );
		g_pd3dDevice->SetFVF( flags );
        g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, header.num_faces );
	}
};

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
CCamera camera;
CCamera pathfind_camera;
CCamera* current_camera;

CInput input;
CEntidad3D entidad1;
agentInstance ag;
ground g;
CNavmesh nav;
CNavmeshQuery nav_query( &nav );

spatialindex sp;
point p( 0, 0, 0 );
point floor_min( -25, -1, -25 );
point floor_max( 25, 0, 25 );
point mpos( 0, 0, 0 );

//-----------------------------------------------------------------------------
// Name: InitD3D()
// Desc: Initializes Direct3D
//-----------------------------------------------------------------------------
void initApp()
{
	srand(timeGetTime());
	sp.Init(-20, -20, 20, 20, 100);

	g.Init();
	
	camera.setProjectionParams( deg2rad( 60.0f ), (float) window_xres / (float) window_yres, 0.1f, 1000.0f );
  camera.lookAt( D3DXVECTOR3( 10, 10, 10 ), D3DXVECTOR3( 0, 0, 0 ), D3DXVECTOR3( 0, 1, 0 ) );
  pathfind_camera.setProjectionParams( deg2rad( 60.0f ), (float)window_xres / (float)window_yres, 0.1f, 1000.0f );
  pathfind_camera.lookAt( D3DXVECTOR3( 0, 40, 5 ), D3DXVECTOR3( 0, 0, 0 ), D3DXVECTOR3( 0, 1, 0 ) );
	entidad1.setLocation( D3DXVECTOR3( -8.5,0,0 ), deg2rad( 60.0f ) );
  current_camera = &camera;
}

//-----------------------------------------------------------------------------
VOID moveCamera( float elapsed ) {

	// Pedir info de donde esta la camara ahora
	D3DXVECTOR3 loc = camera.getPosition( );
	float yaw = camera.getYaw( );
	float pitch = camera.getPitch( );
	
	float rotation_speed = deg2rad( 90 ); // grados/s
	float amount_rotated = rotation_speed * elapsed;
	float movement_speed = 0.1f; // m/s
	float amount_moved = movement_speed * elapsed;

	// Ver si se pulsan teclas
	if( isPressed( 'A' ))
		yaw += amount_rotated;
	if( isPressed( 'D' ))
		yaw -= amount_rotated;
	if( isPressed( 'S' ))
		pitch += amount_rotated;
	if( isPressed( 'X' ))
		pitch -= amount_rotated;

	float max_pitch = deg2rad( 89.5f );
	if( pitch < -max_pitch )
		pitch = -max_pitch;
	if( pitch > max_pitch )
		pitch = max_pitch;

	D3DXVECTOR3 front = yawPitchToAxis( yaw, pitch );

	if( isPressed( 'W' ))
		loc += front * amount_moved;

	D3DXVECTOR3 new_target = loc + front;

	// mover la camara a la nueva posicion
	camera.lookAt( loc, new_target, camera.getUpAuxAxis( ) );
}

VOID controlCamera( float elapsed ) {
  point pnew=entidad1.getPosition();
  pnew=pnew-entidad1.getFront()*2+point(0,1,0);
  p=0.9*p + 0.1*pnew;
  camera.lookAt( p, entidad1.getPosition(), D3DXVECTOR3(0,1,0) );
}

VOID pathfindCamera( float elapsed ) {
  D3DXVECTOR3 loc = pathfind_camera.getPosition( );

  if( isPressed( 'Q' ) )
    loc.y -= 10.f * elapsed;
  if( isPressed( 'E' ) )
    loc.y += 10.f * elapsed;

  loc.y = max( 0.25f, loc.y );

  D3DXVECTOR3 target = pathfind_camera.getTarget( );

  pathfind_camera.lookAt( loc, target, camera.getUpAuxAxis( ) );
}


//-----------------------------------------------------------------------------
void moveEntity( float elapsed ) {

	// rotate
	float amount_rotated = deg2rad( 180.0f ) * elapsed;
	float yaw = entidad1.getYaw();
	if( isPressed( VK_LEFT ))
		yaw += amount_rotated;
	if( isPressed( VK_RIGHT ))
		yaw -= amount_rotated;

	// Move
	D3DXVECTOR3 loc = entidad1.getPosition( );
	float speed = 4.0f;
	float alfa = 0.0f;
	float beta = 0.0f;
	if( isPressed( VK_UP))
		alfa = 1.0f;
	if( isPressed( VK_DOWN ))
		alfa = -1.0f;
	if( alfa != 0.0f )
		loc += entidad1.getFront( ) * alfa * elapsed * speed;
	if( beta != 0.0f )
		loc += entidad1.getLeft( ) * beta * elapsed * speed;

	// Update
	if (g.getGround(loc)!='0') entidad1.setLocation( loc, yaw );
}

void updateMousePos( ) {
  mpos = current_camera->getHitXZOnGround( input.mouse( ).x, input.mouse( ).y, 0.f );
}

//-----------------------------------------------------------------------------
// Name: update()
//-----------------------------------------------------------------------------
void update( float elapsed )
{
  input.update( );
  updateMousePos( );

  if( current_camera == &camera ) {
    controlCamera( elapsed );
    if( input.key( VK_SPACE ).getsPressed( ) )
      current_camera = &pathfind_camera;
  }
  else {
    pathfindCamera( elapsed );
    if( input.key( VK_SPACE ).getsPressed( ) )
      current_camera = &camera;
  }

	moveEntity( elapsed );

  if( input.key( 'N' ).getsPressed( ) ) {
    nav.m_input.clearInput( );
    nav.m_input.addInput( floor_min, floor_max );  // floor
    nav.m_input.addInput( g );  // buildings
    nav.m_input.computeBoundaries( );
    //nav.m_input.aabb_max.y = 0.25f;
  }
  if( input.key( 'M' ).getsPressed( ) ) {
    nav.build( );
  }
  if( input.key( 'R' ).getsPressed( ) ) {
    nav.m_draw_mode = (CNavmesh::EDrawMode) ( (nav.m_draw_mode + 1) % CNavmesh::NAVMESH_DRAW_TYPE_COUNT );
  }
  if( input.key( 'T' ).getsPressed( ) ) {
    nav_query.tool = (CNavmeshQuery::ETool) ((nav_query.tool + 1) % CNavmeshQuery::NUM_TOOLS );
  }
  
  nav_query.update( mpos );
}

//-----------------------------------------------------------------------------
// Name: Render()
//-----------------------------------------------------------------------------
void render()
{
    // Clear the backbuffer to a black color
    g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
                         D3DCOLOR_ARGB( 0, 0, 0, 0 ), 1.0f, 0 );

    // Begin the scene
    if( SUCCEEDED( g_pd3dDevice->BeginScene() ) )
    {
        // Setup the world, view, and projection matrices

		    activateCamera( *current_camera );
		    drawGrid( 20, 1.0f );

		    if (GetAsyncKeyState('G')==0) g.draw();

        drawCube( floor_min, floor_max, D3DCOLOR_RGBA( 64, 64, 64, 255 ) );
        drawCubeWired( nav.m_input.aabb_min, nav.m_input.aabb_max, D3DCOLOR_RGBA( 128, 128, 255, 255 ) );

        nav.render( !isPressed( 'Z' ) );

        drawCube( mpos - D3DXVECTOR3( 0.1, 3.0, 0.1 ), mpos + D3DXVECTOR3( 0.1, 3.0, 0.1 ), D3DCOLOR_RGBA( 255, 255, 255, 255 ) );
        nav_query.render( );

		    entidad1.draw( 0.1, D3DCOLOR_RGBA( 255, 255, 0, 255 ) );
		
        // End the scene
        g_pd3dDevice->EndScene();
    }

    // Present the backbuffer contents to the display
    g_pd3dDevice->Present( NULL, NULL, NULL, NULL );
}



