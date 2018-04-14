#include "common.fx"

#define PI 3.14159265359f


//--------------------------------------------------------------------------------------
// GBuffer generation pass. Vertex
//--------------------------------------------------------------------------------------
void VS_GBufferInstanced(
  in float4 iPos     : POSITION
, in float3 iNormal  : NORMAL0
, in float2 iTex0    : TEXCOORD0
, in float2 iTex1    : TEXCOORD1
, in float4 iTangent : NORMAL1

  // From stream 1 we read the instance information 
, in float4 InstanceWorld0  : TEXCOORD2     // Stream 1
, in float4 InstanceWorld1  : TEXCOORD3     // Stream 1
, in float4 InstanceWorld2  : TEXCOORD4     // Stream 1
, in float4 InstanceWorld3  : TEXCOORD5     // Stream 1
, in float4 InstanceXtras   : TEXCOORD6     // TimeToLife, TimeBlendingOut, ... 

, out float4 oPos      : SV_POSITION
, out float3 oNormal   : NORMAL0
, out float4 oTangent  : NORMAL1
, out float2 oTex0     : TEXCOORD0
, out float2 oTex1     : TEXCOORD1
, out float3 oWorldPos : TEXCOORD2
)
{

  // Build a World matrix from the instance information
  float4x4 instance_world = float4x4(InstanceWorld0, InstanceWorld1, InstanceWorld2, InstanceWorld3 );  

  float4 world_pos = mul(iPos, instance_world);
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
