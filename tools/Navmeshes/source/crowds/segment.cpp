#include "segment.h"


segment::segment()
{
}


segment::segment(point pp1,point pp2)
{
p1.x=pp1.x;
p1.y=pp1.y;
p1.z=pp1.z;
p2.x=pp2.x;
p2.y=pp2.y;
p2.z=pp2.z;
}


void segment::create(point pp1,point pp2)
{
p1=pp1;
p2=pp2;
}


point segment::midpoint()
{
point p((p1.x+p2.x)/2.0,(p1.y+p2.y)/2.0,(p1.z+p2.z)/2.0);
return p;
}


bool segmentcollides(segment s1,segment s2,point &hit)
// watch out: this assumes the lines have y=0, so it's a 2D test!!!!!!
{
float x1=s1.p1.x;
float x2=s1.p2.x;
float x3=s2.p1.x;
float x4=s2.p2.x;
float z1=s1.p1.z;
float z2=s1.p2.z;
float z3=s2.p1.z;
float z4=s2.p2.z;


float den= (z4-z3)*(x2-x1) - (x4-x3)*(z2-z1);
float nua= (x4-x3)*(z1-z3) - (z4-z3)*(x1-x3);
float nub= (x2-x1)*(z1-z3) - (z2-z1)*(x1-x3);

if (fabs(den)<0.001) return false;

float ua=nua/den;
float ub=nub/den;

if (ua>=0.0 && ua<=1.0 && ub>=0.0 && ub<1.0)
	{
	// they collide
	hit.x=x1+ua*(x2-x1);
	hit.y=0;
	hit.z=z1+ua*(z2-z1);
	return true;
	}
return false;
}