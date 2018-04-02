#include "common.fx"

#define PI 3.14159265359f

//--------------------------------------------------------------------------------------
// GBuffer generation pass. Vertex
//--------------------------------------------------------------------------------------
void VS_GBuffer(
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
// GBuffer generation pass. Pixel shader
//--------------------------------------------------------------------------------------
void PS_GBuffer(
  float4 Pos       : SV_POSITION
, float3 iNormal   : NORMAL0
, float4 iTangent  : NORMAL1
, float2 iTex0     : TEXCOORD0
, float2 iTex1     : TEXCOORD1
, float3 iWorldPos : TEXCOORD2
, out float4 o_albedo : SV_Target0
, out float4 o_normal : SV_Target1
, out float1 o_depth  : SV_Target2
)
{
  // Store in the Alpha channel of the albedo texture, the 'metallic' amount of
  // the material
  o_albedo = txAlbedo.Sample(samLinear, iTex0);
  //o_albedo.a = txMetallic.Sample(samLinear, iTex0).r;

  o_normal.xyz = computeNormalMap( iNormal, iTangent, iTex0 );
  o_normal.w = 1;

  // Compute the Z in linear space, and normalize it in the range 0...1
  // In the range z=0 to z=zFar of the camera (not zNear)
  float3 camera2wpos = iWorldPos - camera_pos;
  o_depth = dot( camera_front.xyz, camera2wpos ) / camera_zfar;
}

//--------------------------------------------------------------------------------------
void decodeGBuffer( 
     in float2 iPosition          // Screen coords
   , out float3 wPos 
   , out float3 N 
   , out float3 real_albedo 
   /*
   , out float3 real_specular_color
   , out float  roughness 
   , out float3 reflected_dir
   , out float3 view_dir
   */
   ) {

  int3 ss_load_coords = uint3(iPosition.xy, 0);

  // Recover world position coords
  float  zlinear = txGBufferLinearDepth.Load(ss_load_coords).x;
  wPos = getWorldCoords(iPosition.xy, zlinear);

  // Recuperar la normal en ese pixel. Sabiendo que se
  // guardó en el rango 0..1 pero las normales se mueven
  // en el rango -1..1
  float4 N_rt = txGBufferNormals.Load(ss_load_coords);
  N = decodeNormal( N_rt.xyz );

  // Get other inputs from the GBuffer
  float4 albedo_rt = txGBufferAlbedos.Load(ss_load_coords);
  real_albedo = albedo_rt.xyz;

  /*
  // In the alpha of the albedo, we stored the metallic value
  // and in the alpha of the normal, we stored the roughness
  float  metallic = albedo.a;
         roughness = N_rt.a;
 
  // Apply gamma correction to albedo to bring it back to linear.
  albedo.rgb = pow(albedo.rgb, 2.2f);

  // Lerp with metallic value to find the good diffuse and specular.
  // If metallic = 0, albedo is the albedo, if metallic = 1, the
  // used albedo is almost black
  real_albedo = albedo.rgb * ( 1. - metallic );

  // 0.03 default specular value for dielectric.
  real_specular_color = lerp(0.03f, albedo.rgb, metallic);

  // Eye to object
  float3 incident_dir = normalize(wPos - CameraWorldPos.xyz);
  reflected_dir = normalize(reflect(incident_dir, N));
  view_dir = -incident_dir;
*/

}


//--------------------------------------------------------------------------------------
// Ambient pass, to compute the ambient light of each pixel
float4 PS_ambient(
  in float4 iPosition : SV_Position
) : SV_Target
{
/*
  // Declare some float3 to store the values from the GBuffer
  float3 wPos, N, albedo, specular_color, reflected_dir, view_dir;
  float  roughness;
  decodeGBuffer( iPosition.xy, wPos, N, albedo, specular_color, roughness, reflected_dir, view_dir );

  // if roughness = 0 -> I want to use the miplevel 0, the all-detailed image
  // if roughness = 1 -> I will use the most blurred image, the 8-th mipmap, If image was 256x256 => 1x1
  float mipIndex = roughness * roughness * 8.0f;
  float3 env = txEnvironment.SampleLevel(samLinear, reflected_dir, mipIndex).xyz;
  // Convert the color to linear also.
  env = pow(env, 2.2f);

  // The irrandiance, is read using the N direction.
  // Here we are sampling using the cubemap-miplevel 4, and the already blurred txIrradiance texture
  // and mixing it in base to the scalar_irradiance_vs_mipmaps which comes from the ImGui.
  // Remove the interpolation in the final version!!!
  float3 irradiance_mipmaps = txEnvironment.SampleLevel(samLinear, N, 4).xyz;
  float3 irradiance_texture = txIrradiance.Sample(samLinear, N).xyz;
  float3 irradiance = irradiance_texture * scalar_irradiance_vs_mipmaps + irradiance_mipmaps * ( 1. - scalar_irradiance_vs_mipmaps );

  // How much the environment we see
  float3 env_fresnel = Specular_F_Roughness(specular_color, 1. - roughness * roughness, N, view_dir);
  //return float4(env_fresnel, 1 );

  float g_ReflectionIntensity = 1.0;
  float g_AmbientLightIntensity = 1.0;

  float4 self_illum = txGSelfIllum.Load(uint3(iPosition.xy,0));

  float4 final_color = float4(env_fresnel * env * g_ReflectionIntensity + 
                              albedo.xyz * irradiance * g_AmbientLightIntensity
                              , 1.0f) + self_illum;
  return final_color;
*/
  return float4( 0.2, 0.2, 0.2, 0.f );
}

//--------------------------------------------------------------------------------------
void VS_pass(
  in float4 iPos : POSITION
, in float3 iNormal : NORMAL0
, in float2 iTex0 : TEXCOORD0
, in float2 iTex1 : TEXCOORD0
, in float4 iTangent : NORMAL1
, out float4 oPos : SV_POSITION
) {
  float4 world_pos = mul(iPos, obj_world);
  oPos = mul(world_pos, camera_view_proj);
}


float4 PS_dir_lights( in float4 iPosition : SV_Position ) : SV_Target
{
  float3 wPos, N, albedo;
  decodeGBuffer( iPosition.xy, wPos, N, albedo );

  float shadow_factor = computeShadowFactor( wPos );

  // Diffuse
  float3 Light = light_pos - wPos;
  Light = normalize( Light );
  float diffuseAmount = dot( N, Light );
  diffuseAmount = saturate( diffuseAmount );
  
  float4 light_amount = diffuseAmount * light_color * light_intensity * shadow_factor;

  return light_amount;
}

