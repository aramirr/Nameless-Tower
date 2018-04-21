#ifndef _GROUND_INC
#define _GROUND_INC

#include <d3dx9.h>

class ground
	{
  public:
    int **data;
	  int sx, sz;

	public:
		void Init();
		int getGround(int, int);
		int getGround(D3DXVECTOR3);
		void draw();
	};


#endif