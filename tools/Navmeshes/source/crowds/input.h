#ifndef _INPUT_INC
#define _INPUT_INC

#include <vector>

class CInput
	{
  struct TKey{
    int   key;
    bool  curr;
    bool  prev;
    bool isPressed( ) const { return curr; }
    bool getsPressed( ) const { return !prev && curr; }
    bool getsReleased( ) const { return prev && !curr; }
  };
  std::vector< TKey > keys;

  struct TMouse {
    float x;
    float y;
    bool  curr;
    bool  prev;
    bool isPressed( ) const { return curr; }
    bool getsPressed( ) const { return !prev && curr; }
    bool getsReleased( ) const { return prev && !curr; }
  };
  TMouse _mouse;
  
	public:
    CInput( );
    void update( );
    TKey& key( int );
    TMouse& mouse( );
	};


#endif