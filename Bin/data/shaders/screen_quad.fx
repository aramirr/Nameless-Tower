#include "common.fx"

/// --------------------
struct VS_FULL_OUTPUT {
  float4 Pos   : SV_POSITION;
  float2 UV    : TEXCOORD0;
};

// ----------------------------------------
VS_FULL_OUTPUT VS(
  float3 iPos   : POSITION,     // 0..1, 0..1, 0 en la z
  float4 iColor : COLOR0
  )
{
  VS_FULL_OUTPUT output = (VS_FULL_OUTPUT)0;
  output.Pos = float4(iPos.x * 2 - 1., 1 - iPos.y * 2, 0.5, 1);
  output.UV  = iPos.xy;
  return output;
}

// ----------------------------------------
float4 PS(
  VS_FULL_OUTPUT input
  ) : SV_Target
{
  float4 self_illum = txSelfIllum.Load(uint3(input.Pos.xy, 0));
  float4 oDiffuse = txAlbedo.Sample(samLinear, input.UV);
    float4 final_color = oDiffuse + float4(self_illum.xyz, 1) * 0.1;
    return oDiffuse;
}