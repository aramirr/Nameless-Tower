#ifndef _SEGMENT_INC
#define _SEGMENT_INC

#include <Windows.h>
#include <d3dx9.h>

typedef D3DXVECTOR3 point;

class segment
	{
	public:
		point p1,p2;
		segment();
		segment(point,point);
		void create(point,point);
		point midpoint();
	}; 

bool segmentcollides(segment,segment,point &);

#endif
