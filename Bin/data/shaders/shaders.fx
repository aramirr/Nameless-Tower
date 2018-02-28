//--------------------------------------------------------------------------------------
#include "ctes.h"

//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
  float4 Pos : SV_POSITION;
  float4 Color : COLOR0;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(float4 Pos : POSITION, float4 Color : COLOR)
{
  VS_OUTPUT output = (VS_OUTPUT)0;
  output.Pos = mul(Pos, obj_world);
  output.Pos = mul(output.Pos, camera_view);
  output.Pos = mul(output.Pos, camera_proj);
  output.Color = Color * obj_color;
  return output;
}

VS_OUTPUT VS_Objs(
	 float4 Pos : POSITION
   , float3 N : NORMAL
   , float2 UV : TEXCOORD0
   )
{
  VS_OUTPUT output = (VS_OUTPUT)0;
  output.Pos = mul(Pos, obj_world);
  output.Pos = mul(output.Pos, camera_view);
  output.Pos = mul(output.Pos, camera_proj);
  output.Color = obj_color;
  return output;
}

VS_OUTPUT VS_Objs_uv2(
	 float4 Pos : POSITION
   , float3 N : NORMAL
   , float2 UV : TEXCOORD0
   , float2 UV2 : TEXCOORD1
   )
{
  VS_OUTPUT output = (VS_OUTPUT)0;
  output.Pos = mul(Pos, obj_world);
  output.Pos = mul(output.Pos, camera_view);
  output.Pos = mul(output.Pos, camera_proj);
  output.Color = obj_color;
  return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(VS_OUTPUT input) : SV_Target
{
  return input.Color;
}