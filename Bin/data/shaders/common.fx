//--------------------------------------------------------------------------------------
#include "ctes.h"

//--------------------------------------------------------------------------------------
Texture2D    txAlbedo         SLOT( TS_ALBEDO );

Texture2D    txLightProjector SLOT( TS_LIGHT_PROJECTOR );
Texture2D    txLightShadowMap SLOT( TS_LIGHT_SHADOW_MAP );

//--------------------------------------------------------------------------------------
SamplerState samLinear        : register(s0);
SamplerState samBorderLinear  : register(s1);
SamplerComparisonState samPCF : register(s2);

//--------------------------------------------------------------------------------------
float4x4 getSkinMtx( int4 iBones, float4 iWeights ) {
  // This matrix will be reused for the position, Normal, Tangent, etc
  return  Bones[iBones.x] * iWeights.x
        + Bones[iBones.y] * iWeights.y
        + Bones[iBones.z] * iWeights.z
        + Bones[iBones.w] * iWeights.w;
}

