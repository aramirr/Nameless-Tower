#include "common.fx"

/// --------------------
struct VS_FULL_OUTPUT {
  float4 Pos : SV_POSITION;
  float2 UV : TEXCOORD0;
  float4 Color : TEXCOORD1;
};

// ----------------------------------------
VS_FULL_OUTPUT VS_Particles(
  float4 iPos   : POSITION,     // 0..1, 0..1, 0 en la z
  float4 iColor : COLOR0
  )
{
  VS_FULL_OUTPUT output = (VS_FULL_OUTPUT)0;
  float4 pos = iPos - float4(0.5, 0.5, 0, 0);
  float4 world_pos = mul(pos, obj_world);
  output.Pos = mul(world_pos, camera_view_proj);
  output.UV  = iPos.xy;
  output.Color = float4( 1,1,1,1);
  return output;
}

// ----------------------------------------
float4 PS_Particles(
  VS_FULL_OUTPUT input
  ) : SV_Target
{

  float4 texture_color = txAlbedo.Sample(samLinear, input.UV) * input.Color;
  return texture_color;
}

