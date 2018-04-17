#include <Windows.h>
#include <windowsx.h>
#include <d3dx9.h>
#include "draw_utils.h"
#include "input.h"

int window_xres = 1920;
int window_yres = 1080;

extern void initApp();
extern void render();
extern void update( float elapsed );
extern CInput input;

//-----------------------------------------------------------------------------
bool isPressed( int key ) {
	return ( ::GetAsyncKeyState( key ) & 0x8000 ) == 0x8000;
}

//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: The window's message handler
//-----------------------------------------------------------------------------
LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch( msg )
    {
      case WM_LBUTTONDOWN:
      input.mouse( ).x = (float)GET_X_LPARAM( lParam ) / (float)window_xres;
      input.mouse( ).y = (float)GET_Y_LPARAM( lParam ) / (float)window_yres;
      input.mouse( ).curr = true;
      return 0;

      case WM_MOUSEMOVE:
      input.mouse( ).x = (float)GET_X_LPARAM( lParam ) / (float)window_xres;
      input.mouse( ).y = (float)GET_Y_LPARAM( lParam ) / (float)window_yres;
      return 0;

      case WM_LBUTTONUP:
      input.mouse( ).curr = false;
      return 0;

      case WM_DESTROY:
          deinitD3D();
          PostQuitMessage( 0 );
       return 0;
    }

    return DefWindowProc( hWnd, msg, wParam, lParam );
}


//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: The application's entry point
//-----------------------------------------------------------------------------
INT WINAPI wWinMain( HINSTANCE hInst, HINSTANCE, LPWSTR, INT )
{
    UNREFERENCED_PARAMETER( hInst );

    // Register the window class
    WNDCLASSEX wc =
    {
        sizeof( WNDCLASSEX ), CS_CLASSDC, MsgProc, 0L, 0L,
        GetModuleHandle( NULL ), NULL, NULL, NULL, NULL,
        "D3D Tutorial", NULL
    };
    RegisterClassEx( &wc );

    // Create the application's window
    HWND hWnd = CreateWindow( "D3D Tutorial", "D3D Tutorial 03: Matrices",
                        WS_OVERLAPPEDWINDOW, 100, 100, window_xres, window_yres,
                              NULL, NULL, wc.hInstance, NULL );

    // Initialize Direct3D
    if( SUCCEEDED( initD3D( hWnd ) ) )
    {
		initApp();

        // Show the window
        ShowWindow( hWnd, SW_SHOWDEFAULT );
        UpdateWindow( hWnd );

        // Enter the message loop
        MSG msg;
        ZeroMemory( &msg, sizeof( msg ) );
        while( msg.message != WM_QUIT )
        {
            if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
            {
                TranslateMessage( &msg );
                DispatchMessage( &msg );
            }
			else {
				float elapsed = 1.0f / 60.0f;
                update( elapsed );
				render();
			}
        }
    }

    UnregisterClass( "D3D Tutorial", wc.hInstance );
    return 0;
}


