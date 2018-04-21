#include <math.h>
#include "spatialindex.h"


void spatialindex::Init(float x1, float z1, float x2, float z2, int pncells)
{
pmin.x = x1;
pmin.z = z1;
pmax.x = x2;
pmax.z = z2;
ncells = pncells;

// begin memory allocation
sp = new vrulecontroller*[ncells];
for (int x = 0; x < ncells; ++x) 
	{
	sp[x] = new vrulecontroller[ncells];
	for (int y = 0; y < ncells; ++y) 
		{
		// accessing the list itself would be: sp[x][y].push_back(kk);
		}
	}

querybuffer.resize(256);
}


void spatialindex::Insert(rulecontroller *r)
{
D3DXVECTOR3 pos = r->ent->getPosition();
pos = pos - pmin;
D3DXVECTOR3 width= pmax-pmin;
int cellx = ncells*pos.x / width.x;
int cellz = ncells*pos.z / width.z;
if (cellx < 0) cellx = 0;
if (cellx >= ncells) cellx = ncells - 1;
if (cellz < 0) cellz = 0;
if (cellz >= ncells) cellz = ncells - 1;

sp[cellx][cellz].push_back(r);
}


void spatialindex::Delete(rulecontroller *r)
{
D3DXVECTOR3 pos = r->ent->getPosition();
pos = pos - pmin;
D3DXVECTOR3 width = pmax - pmin;
int cellx = ncells*pos.x / width.x;
int cellz = ncells*pos.z / width.z;
if (cellx < 0) cellx = 0;
if (cellx >= ncells) cellx = ncells-1;
if (cellz < 0) cellz = 0;
if (cellz >= ncells) cellz = ncells - 1;

list<rulecontroller *>::iterator cand = sp[cellx][cellz].begin();
int i = 0;
int found = -1;
while (cand != sp[cellx][cellz].end())
	{ 
	if (*cand==r)
		{
		int size1 = sp[cellx][cellz].size();
		sp[cellx][cellz].erase(cand);
		int size2 = sp[cellx][cellz].size();
		return;
		}
	cand++;
	}
}


void spatialindex::Move(rulecontroller *r,float dx, float dz)
{
D3DXVECTOR3 pos = r->ent->getPosition();
pos = pos - pmin;
D3DXVECTOR3 width = pmax - pmin;
int oldcellx = ncells*pos.x / width.x;
int oldcellz = ncells*pos.z / width.z;

if (oldcellx < 0) oldcellx = 0;
if (oldcellx >= ncells) oldcellx = ncells - 1;
if (oldcellz < 0) oldcellz = 0;
if (oldcellz >= ncells) oldcellz = ncells - 1;


pos = r->ent->getPosition();
pos.x += dx;
pos.z += dz;
pos = pos - pmin;
int newcellx = ncells*pos.x / width.x;
int newcellz = ncells*pos.z / width.z;

if (newcellx < 0) newcellx = 0;
if (newcellx >= ncells) newcellx = ncells - 1;
if (newcellz < 0) newcellz = 0;
if (newcellz >= ncells) newcellz = ncells - 1;


// movement needed
// step 1: delete from old location
if (newcellx != oldcellx || newcellz != oldcellz) Delete(r);

// in any case, change coordinates
pos = r->ent->getPosition();
pos.x += dx;
pos.z += dz;
float yaw = r->ent->getYaw();
r->ent->setLocation(pos,yaw);

// step 2: move and insert into new location
if (newcellx != oldcellx || newcellz != oldcellz) Insert(r);
}


bool spatialindex::QueryCloserThan(float x, float z, float radius)
{
float radius2 = radius*radius;
querybuffer[0] = NULL;
D3DXVECTOR3 mypos(x,0,z);
mypos = mypos - pmin;
D3DXVECTOR3 width = pmax - pmin;
int cellx = ncells*mypos.x / width.x;
int cellz = ncells*mypos.z / width.z;

mypos.x = x;
mypos.z = z;

float cellsize = (pmax.x - pmin.x) / ncells;

cellsize = radius / cellsize;

int cellx1 = max(cellx - cellsize-1, 0);
int cellz1 = max(cellz - cellsize - 1, 0);
int cellx2 = min(cellx + cellsize + 1, ncells - 1);
int cellz2 = min(cellz + cellsize+ 1, ncells - 1);

int pos = 0;
for (int x = cellx1; x < cellx2;x++)
	{
	for (int z = cellz1; z < cellz2; z++)
		{
		list<rulecontroller *>::iterator cand = sp[x][z].begin();
		list<rulecontroller *>::iterator ends = sp[x][z].end();
		int size = sp[x][z].size();
		while (cand != ends)
			{
			D3DXVECTOR3 hispos = (*cand)->ent->getPosition();
			D3DXVECTOR3 deltapos = mypos - hispos;
//			float distance = sqrt(deltapos.x*deltapos.x + deltapos.y*deltapos.y + deltapos.z*deltapos.z);
// the next is to avoid the sqrt
			float distance = deltapos.x*deltapos.x + deltapos.y*deltapos.y + deltapos.z*deltapos.z;

			if (distance < radius2 && distance > 0.001)
				{
				querybuffer[pos] = (*cand);
				pos++;
				}
			cand++;
			}
		}
	}
querybuffer[pos] = NULL;
return (pos > 0);
}


int spatialindex::countNodes()
{
int count = 0;

for (int x = 0; x < ncells; ++x)
{
	for (int y = 0; y < ncells; ++y)
	{
		count+=sp[x][y].size();
	}
}
return count;
}
