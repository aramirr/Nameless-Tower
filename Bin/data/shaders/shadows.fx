#include "common.fx"

struct VS_OUTPUT
{
  float4 Pos   : SV_POSITION;
  // To generate the shadows I don't need the UV's or the normal
};

// ----------------------------------------------
VS_OUTPUT VS(
  float4 iPos : POSITION
  )
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    float4 world_pos = mul( iPos, obj_world );
    output.Pos = mul(world_pos, camera_view );
    output.Pos = mul(output.Pos, camera_proj );
    return output;
}

// ----------------------------------------------
VS_OUTPUT VS_Instanced(
    in float4 iPos : POSITION

  // From stream 1 we read the instance information 
  , in float4 InstanceWorld0 : TEXCOORD2     // Stream 1
  , in float4 InstanceWorld1 : TEXCOORD3     // Stream 1
  , in float4 InstanceWorld2 : TEXCOORD4     // Stream 1
  , in float4 InstanceWorld3 : TEXCOORD5     // Stream 1
)
{
  // Build a World matrix from the instance information
  float4x4 instance_world = float4x4(InstanceWorld0, InstanceWorld1, InstanceWorld2, InstanceWorld3);

  VS_OUTPUT output = (VS_OUTPUT)0;
  float4 world_pos = mul(iPos, instance_world);
  output.Pos = mul(world_pos, camera_view);
  output.Pos = mul(output.Pos, camera_proj);
  return output;
}

// -----------------------------------------------------
VS_OUTPUT VS_Skin(
    float4 iPos : POSITION
  , float3 iN   : NORMAL
  , float2 iUV  : TEXCOORD0
  , int4   iBones   : BONES
  , float4 iWeights : WEIGHTS
  )
{

  float4x4 skin_mtx = getSkinMtx( iBones, iWeights );

  // Skinned pos
  float4 world_pos = mul(iPos, skin_mtx);
  VS_OUTPUT output = (VS_OUTPUT)0;
  output.Pos = mul(world_pos, camera_view );
  output.Pos = mul(output.Pos, camera_proj );
  return output;
}
