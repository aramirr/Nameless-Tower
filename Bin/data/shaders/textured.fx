//--------------------------------------------------------------------------------------
#include "common.fx"

//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
  float4 Pos : SV_POSITION;
  float3 N    : NORMAL0;
  float2 UV   : TEXCOORD0;
  float3 wPos : TEXCOORD1;
  float4 T    : NORMAL1;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(
  float4 iPos : POSITION
, float3 iN   : NORMAL0
, float2 iUV : TEXCOORD0
, float2 iUV2 : TEXCOORD1
, float4 iTan : NORMAL1
)

{
  VS_OUTPUT output = (VS_OUTPUT)0;
  float4 world_pos = mul(iPos, obj_world);
  output.wPos = world_pos.xyz;
  output.Pos = mul(world_pos, camera_view);
  output.Pos = mul(output.Pos, camera_proj);
  // Rotate the normal
  output.N = mul(iN, (float3x3)obj_world);
  output.T.xyz = mul(iTan.xyz, (float3x3)obj_world);
  output.T.w = iTan.w;      // Keep the w as is
  output.UV = iUV;
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
  // ------------------------------
  // This needs fixing!!!
  output.T = float4(output.N,1);
  return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(VS_OUTPUT input) : SV_Target
{

  //input.UV *= 4;

  float3 wN = computeNormalMap( input.N, input.T, input.UV );

  float shadow_factor = computeShadowFactor( input.wPos );

  // Convert pixel position in world space to light space
  float4 pos_in_light_proj_space = mul( float4(input.wPos,1), light_view_proj_offset );
  float3 pos_in_light_homo_space = pos_in_light_proj_space.xyz / pos_in_light_proj_space.w; // -1..1
  
  // Use these coords to access the projector texture of the light dir
  float4 light_projector_color = txLightProjector.Sample(samBorderLinear, pos_in_light_homo_space.xy );
  //if( pos_in_light_proj_space.z < 0. )
  //  light_projector_color = float4(0,0,0,0);

  // Fade to zero in the last 1% of the zbuffer of the light
  light_projector_color *= smoothstep( 1.0f, 0.99f, pos_in_light_homo_space.z );

  // Projector disabled with this line!!
  light_projector_color = float4(1,1,1,1);

  // Diffuse amount N.L
  float3 Light = light_pos - input.wPos;
  Light = normalize( Light );
  float diffuseAmount = dot( wN, Light );
  diffuseAmount = saturate( 0.2+ diffuseAmount );

  // Sample in the direction of the N with a bias
  float4 env_color = txEnvironmentMap.SampleBias(samLinear, wN, 5 );

  // Sample in the reflected direction of the eye
  float3 eye = camera_pos - input.wPos;
  float3 eye_refl = reflect( -eye, wN );
  float4 env_color_refl = txEnvironmentMap.Sample(samLinear, eye_refl );

  float light_amount = diffuseAmount * shadow_factor;

  // Add a minimum of 0.25 of light
  light_amount = 0.25 + light_amount * 0.75;

  float4 texture_color = txAlbedo.Sample(samLinear, input.UV);
  
  return texture_color * obj_color * light_amount
        * light_color * light_projector_color * light_intensity 
        ;
}