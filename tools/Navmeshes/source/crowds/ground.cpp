#include <stdio.h>
#include <stdlib.h>
#include "ground.h"
#include "draw_utils.h"

/*
KEY:
0: forbidden zone
1: traverse east-west
2: traverse north-south
3: traverse both directions
*/

void ground::Init()
{
	FILE *f;
	
fopen_s(&f,"map.txt", "rt");

sx = 40;
sz = 40;

data= new int*[sx];
char c;

for (int x = 0; x < sx; ++x)
{
	data[x] = new int[sz];
	for (int z = 0; z < sz; ++z)
	{
		fread(&c, 1, 1, f);
		data[x][z] = c;
		// accessing the list itself would be: sp[x][y].push_back(kk);
	}
	fread(&c, 1, 1, f); // cr lf

}

}


void ground::draw()
{
for (int x = 0; x < sx; ++x)
	{
	for (int z = 0; z < sz; ++z)
		{
		int c = data[x][z];
		if (c == '0')
			{
      D3DXVECTOR3 p0 = D3DXVECTOR3( -20 + x, 0, -20 + z );
      D3DXVECTOR3 p1 = D3DXVECTOR3( -20 + x + 1, 0.5, -20 + z + 1 ); 
      drawBuilding( p0, p1, D3DCOLOR_RGBA( 255, 0, 0, 128 ) );
			//drawLine(D3DXVECTOR3(-20 + x + 1, 0, -20 + z), D3DXVECTOR3(-20 + x, 0, -20 + z + 1), D3DCOLOR_RGBA(255, 0, 0, 255));
			}
		}
	}
}


int ground::getGround(int x, int z)
{
	if (x < 0) x = 0;
	if (z < 0) z = 0;
	if (x >= sx) x = sx-1;
	if (z >= sz) z = sz-1;


return data[x][z];
}


int ground::getGround(D3DXVECTOR3 p)
{
	float pminx = -(sx / 2);
	float pminz = -(sz / 2);

	int cx = (p.x - pminx);
	int cz = (p.z - pminz);
	return getGround(cx, cz);
}

