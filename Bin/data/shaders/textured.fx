//--------------------------------------------------------------------------------------
#include "common.fx"

//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
  float4 Pos : SV_POSITION;

  float3 N    : NORMAL;
  float2 UV   : TEXCOORD0;
  float2 UVB  : TEXCOORD1;
  float3 wPos : TEXCOORD2;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(

  float4 iPos : POSITION
, float3 iN   : NORMAL
, float2 iUV  : TEXCOORD0
, float2 iUVB : TEXCOORD1
)

{
  VS_OUTPUT output = (VS_OUTPUT)0;
  float4 world_pos = mul(iPos, obj_world);
  output.wPos = world_pos.xyz;
  output.Pos = mul(world_pos, camera_view);
  output.Pos = mul(output.Pos, camera_proj);
  // Rotate the normal

  output.N = mul(iN, (float3x3)obj_world);
  output.UV = iUV;
  output.UVB = iUVB;
  return output;
}

VS_OUTPUT VS_Skin(
  float4 iPos : POSITION
, float3 iN   : NORMAL
, float2 iUV  : TEXCOORD0
, int4   iBones   : BONES
, float4 iWeights : WEIGHTS
)
{

  float4x4 skin_mtx = getSkinMtx( iBones, iWeights );
  float4 skinned_Pos = mul(iPos, skin_mtx);

  VS_OUTPUT output = (VS_OUTPUT)0;
  output.wPos = skinned_Pos.xyz;
  // The world matrix is inside the bones matrixs
  output.Pos = mul(skinned_Pos, camera_view);
  output.Pos = mul(output.Pos, camera_proj);
  // Rotate the normal
  output.N = mul(iN, (float3x3)obj_world);
  output.UV = iUV;
  output.UVB = iUV;
  return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
Texture2D    txDiffuse      : register(t0);
Texture2D    txLightMap			: register(t1);
//Texture2D    txLightProjector : register(t2);  // TS_LIGHT_PROJECTOR

//SamplerState samLinear       : register(s0);
//SamplerState samBorderLinear : register(s1);

float4 PS(VS_OUTPUT input) : SV_Target
{
	//float4 lightmap_color = txLightMap.Sample(samLinear, input.UVB);
	//return lightmap_color;

  // Convert pixel position in world space to light space
  float4 pos_in_light_proj_space = mul( float4(input.wPos,1), light_view_proj_offset );
  float3 pos_in_light_homo_space = pos_in_light_proj_space.xyz / pos_in_light_proj_space.w; // -1..1
  
  // Use these coords to access the projector texture of the light dir
  float4 light_projector_color = txLightProjector.Sample(samBorderLinear, pos_in_light_homo_space.xy );
  if( pos_in_light_proj_space.z < 0. )
    light_projector_color = float4(0,0,0,0);

  // Fade to zero in the last 1% of the zbuffer of the light
  light_projector_color *= smoothstep( 1.0f, 0.99f, pos_in_light_homo_space.z );

  float3 Light = light_pos - input.wPos;
  Light = normalize( Light );
  float diffuseAmount = dot( input.N, Light );
  diffuseAmount = saturate( 0.2 + diffuseAmount );
  diffuseAmount = 0.5 + diffuseAmount;

  float4 texture_color = txDiffuse.Sample(samLinear, input.UV);
  return texture_color * obj_color * diffuseAmount * light_color * light_intensity;

}