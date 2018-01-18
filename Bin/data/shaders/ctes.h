
#define CB_CAMERAS   0

#ifdef WIN32

#define CB_DEF(xName)   struct xName

#else

#define CB_DEF(xName)   cbuffer xName : register(b0)
// cbuffer ConstantBuffer : register(b0)
#define MAT44                     matrix

#endif

CB_DEF(CCteCamera)
{
  MAT44 world;
  MAT44 view;
  MAT44 proj;
};
