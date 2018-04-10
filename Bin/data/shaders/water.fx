#include "common.fx"

//--------------------------------------------------------------------------------------
void VS(
  in float4 iPos     : POSITION
, in float3 iNormal  : NORMAL0
, in float2 iTex0    : TEXCOORD0
, in float2 iTex1    : TEXCOORD1
, in float4 iTangent : NORMAL1

, out float4 oPos      : SV_POSITION
, out float3 oNormal   : NORMAL0
, out float4 oTangent  : NORMAL1
, out float2 oTex0     : TEXCOORD0
, out float2 oTex1     : TEXCOORD1
, out float3 oWorldPos : TEXCOORD2
)
{
  float4 world_pos = mul(iPos, obj_world);
  oPos = mul(world_pos, camera_view_proj);

  // Rotar la normal segun la transform del objeto
  oNormal = mul(iNormal, (float3x3)obj_world);
  oTangent.xyz = mul(iTangent.xyz, (float3x3)obj_world);
  oTangent.w = iTangent.w;

  // Las uv's se pasan directamente al ps
  oTex0 = iTex0;
  oTex1 = iTex1;
  oWorldPos = world_pos.xyz;
}

//--------------------------------------------------------------------------------------
float4 PS(
  float4 iPosition : SV_POSITION    // Screen coords
, float3 iNormal   : NORMAL0
, float4 iTangent  : NORMAL1
, float2 iTex0     : TEXCOORD0
, float2 iTex1     : TEXCOORD1
, float3 iWorldPos : TEXCOORD2
)  : SV_Target0
{

  int3 ss_load_coords = uint3(iPosition.xy, 0);

  // Obtain a random value associated to each pos in the surface
  float4 noise0 = txNoiseMap.Sample( samLinear, iTex0 * 12 + global_world_time * float2(1,0) ) * 2 - 1;  // Noise in the -1..1 range

  // Modify the position where we take the sample
  float3 surface_pos = iWorldPos + noise0.xyz * .5;

  // Convert world coords to camera space in the range 0..1 to access the rt_main texture
  float4 pos_proj_space = mul( float4(surface_pos,1), camera_view_proj );
  float3 pos_homo_space = pos_proj_space.xyz / pos_proj_space.w;    // -1..1
  float2 pos_camera_unit_space = float2( ( 1 + pos_homo_space.x ) * 0.5, ( 1 - pos_homo_space.y ) * 0.5 );
  float4 color_base = txAlbedo.Sample(samClampLinear, pos_camera_unit_space);
  return color_base * 0.5;
}
