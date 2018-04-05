#ifndef _SPATIALINDEX_INC
#define _SPATIALINDEX_INC

#include <d3d9.h>
#include <d3dx9.h>
#include <vector>
#include <list>
#include "rulecontroller.h"

using namespace std;

typedef list<rulecontroller *> vrulecontroller;

class spatialindex
	{
	D3DXVECTOR3 pmin, pmax;
	int ncells;
	
	vrulecontroller** sp;
	public:
		vector<rulecontroller *> querybuffer;
		void Init(float, float, float, float, int);
		void Insert(rulecontroller *r);
		void Delete(rulecontroller *r);
		void Move(rulecontroller *r,float,float);
		bool QueryCloserThan(float, float, float); // true if any;
		int countNodes();
	};


#endif