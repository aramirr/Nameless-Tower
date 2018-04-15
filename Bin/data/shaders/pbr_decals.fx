#include "common.fx"

//--------------------------------------------------------------------------------------
struct VS_TEXTURED_OUTPUT
{
  float4 Pos:     SV_POSITION;
  float3 center:  TEXCOORD0;
  float3 decal_x: TEXCOORD1;
  float3 decal_z: TEXCOORD2;
  float  opacity: TEXCOORD3;
  float2 uv:      TEXCOORD4;
  float4 color:   TEXCOORD5;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_TEXTURED_OUTPUT VS(

  // From stream 0 we read the instanced mesh (the billboard)
  in float3 iPos     : POSITION           // Stream 0
, in float3 iNormal  : NORMAL0
, in float2 iTex0    : TEXCOORD0
, in float2 iTex1    : TEXCOORD1
, in float4 iTangent : NORMAL1

  // From stream 1 we read the instance information 
, in TInstanceWorldData instance_data     // Stream 1
, in float4 InstanceXtras   : TEXCOORD6     // TimeToLife, TimeBlendingOut, ...  
)
{
  VS_TEXTURED_OUTPUT output = (VS_TEXTURED_OUTPUT)0;
  
  // Build a World matrix from the instance information
  float4x4 instance_world = getWorldOfInstance(instance_data);

  // Transform local vertex of the box to the world coordinates
  float4 world_pos = mul(float4(iPos,1), instance_world);
 
  // Concatenate with the instance world transform  
  // world_pos = mul(world_pos, obj_world);

  // Transform it to projective space
	output.Pos = mul( world_pos, camera_view_proj );

  output.center  = float3( instance_world[3][0], instance_world[3][1], instance_world[3][2] );
  output.decal_x = float3( instance_world[0][0], instance_world[0][1], instance_world[0][2] );
  output.decal_z = float3( instance_world[2][0], instance_world[2][1], instance_world[2][2] );

  // Blendout in the last TimeBlendingOut secs of TimeToLife
  //float TimeToLife = InstanceXtras.x;
  //float TimeBlendingOut = InstanceXtras.y;
  //output.opacity = smoothstep( 0, TimeBlendingOut, TimeToLife );
  output.opacity = 1;
  output.color = InstanceXtras;

  output.uv = iTex0;

  return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
void PS(
  //, in float4 iPosition : SV_Position
    in VS_TEXTURED_OUTPUT input

  , out float4 o_albedo : SV_Target0
  //, out float4 o_normal : SV_Target1
  //, out float4 o_self_illum : SV_Target2
  )
{

  //o_albedo = txAlbedo.Sample(samLinear, input.uv);

  float2 iPosition = input.Pos;

  int3 ss_load_coords = uint3(iPosition.xy, 0);

  // Recuperar la posicion de mundo para ese pixel
  float  zlinear = txGBufferLinearDepth.Load(ss_load_coords).x;
  float3 wPos = getWorldCoords(iPosition.xy, zlinear);

  float3 decal_x = input.decal_x;
  float3 decal_z = input.decal_z;

  float  decal_scale = 0.5 / dot( decal_x, decal_x );

  float3 decal_center = input.center;
  float3 decal_top_left = decal_center - decal_x - decal_z;

  float3 decal_top_left_to_wPos = wPos - decal_top_left;
  float amount_of_x = dot( decal_top_left_to_wPos, decal_x) * decal_scale; 
  float amount_of_z = dot( decal_top_left_to_wPos, decal_z) * decal_scale;

  float distance_to_decal_center = length( wPos - decal_center);

  //o_albedo = float4( distance_to_decal_center,distance_to_decal_center,distance_to_decal_center,1);

  float4 decal_color = txAlbedo.Sample(samBorderLinear, float2(amount_of_x,amount_of_z));
  //float4 decal_normal = txNormal.Sample(samLinear, float2(amount_of_x,amount_of_z)) * 2.0 - 1.0;
  //o_normal = decal_normal;
  //o_albedo = decal_color;
  //o_albedo.xyz = abs(wPos.x - (int)wPos.x) * abs(wPos.z - (int)wPos.z); o_albedo.w = 1;
  //o_albedo = float4(decal_color.xyz,1 );
  //o_albedo = float4( amount_of_x, amount_of_z, 0, 1 );

  //o_albedo = decal_color;
  //o_albedo.a *= input.opacity;

  o_albedo.xyz = input.color;
  o_albedo.a = decal_color.a;
  //o_normal.a = decal_color.a;
  // To 'see' the boxes
  //o_albedo.a += 0.3;
 
  //if( (input.uv.x < 0.01 || input.uv.x > 0.99 ) || (input.uv.y < 0.01 || input.uv.y > 0.99 ) )
  //  o_albedo.a = 1.0;

  //o_normal = float4(1,1,0,1);
  //o_self_illum = float4(1,1,0,1);
}

