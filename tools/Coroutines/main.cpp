#include <conio.h>
#include "SLB/SLB.hpp"
#include "input.h"
#include <iostream>

// ---------------------------------------------------
CInput input;
SLB::Manager m;
SLB::Script s(&m);

bool isPressed(int key) {
  return (GetAsyncKeyState(key) & 0x8000) != 0;
}

void runFile(const char* filename)
{
  try
  {
    s.doFile(filename);
  }
  catch(std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  };
}

void runCode(const char* code)
{
  try
  {
    s.doString(code);
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  };
}
// ---------------------------------------------------

void main()
{
  printf( "Welcome\n" );
  printf( "Scripting ready\n" );
  _getch( );

  runFile("coroutines.lua");
  
  char buffer[64];
  float t = 1.f / 60.f;
  while( true ) {
    input.update( );

    sprintf(buffer, "updateCoroutines(%f)", t);
    runCode(buffer);

    if (input.key(VK_F4).getsPressed())
      runFile("test.lua");
    if (input.key(VK_F3).getsPressed())
      runFile("test_2.lua");

    if( input.key( VK_F1 ).getsReleased( ) )
      break;

    Sleep(t * 1000);
  }

  printf( "Bye\n" );
  _getch( );
}

