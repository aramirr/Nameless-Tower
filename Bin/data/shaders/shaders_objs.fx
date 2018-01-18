//--------------------------------------------------------------------------------------
#include "ctes.h"

//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
  float4 Pos : SV_POSITION;
  float3 N : NORMAL;
  float2 UV : UV;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(
   float4 Pos : POSITION
  , float3 N : NORMAL
  , float2 UV : TEXCOORD0
)

{
  VS_OUTPUT output = (VS_OUTPUT)0;
  output.Pos = mul(Pos, world);
  output.Pos = mul(output.Pos, view);
  output.Pos = mul(output.Pos, proj);
  output.N = N;
  output.UV = UV;
  return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(VS_OUTPUT input) : SV_Target
{
  return float4(input.UV, 0, 1 );
}