#include "common.fx"

#define PI 3.14159265359f

void VS(
  in float4 iPos : POSITION
, in float3 iNormal : NORMAL0
, in float2 iTex0 : TEXCOORD0
, in float2 iTex1 : TEXCOORD1
, in float4 iTangent : NORMAL1

, out float4 oPos : SV_POSITION
, out float3 oNormal : NORMAL0
, out float4 oTangent : NORMAL1
, out float2 oTex0 : TEXCOORD0
, out float2 oTex1 : TEXCOORD1
, out float3 oWorldPos : TEXCOORD2
)
{
    float4 world_pos = mul(iPos, obj_world);
    oPos = mul(world_pos, camera_view_proj);

  // Rotar la normal segun la transform del objeto
    oNormal = mul(iNormal, (float3x3) obj_world);
    oTangent.xyz = mul(iTangent.xyz, (float3x3) obj_world);
    oTangent.w = iTangent.w;

  // Las uv's se pasan directamente al ps
    oTex0 = iTex0;
    oTex1 = iTex1;
    oWorldPos = world_pos.xyz;
}

void VS_Skin(
  in float4 iPos : POSITION
, in float3 iNormal : NORMAL0
, in float2 iTex0 : TEXCOORD0
, in float2 iTex1 : TEXCOORD1
, in float4 iTangent : NORMAL1
, in int4 iBones : BONES
, in float4 iWeights : WEIGHTS

, out float4 oPos : SV_POSITION
, out float3 oNormal : NORMAL0
, out float4 oTangent : NORMAL1
, out float2 oTex0 : TEXCOORD0
, out float2 oTex1 : TEXCOORD1
, out float3 oWorldPos : TEXCOORD2
)
{
    float4x4 skin_mtx = getSkinMtx(iBones, iWeights);
    //float4 skinned_Pos = mul(float4(iPos.xyz * BonesScale, 1), skin_mtx);
    float4 skinned_Pos = mul(iPos, skin_mtx);

    oWorldPos = skinned_Pos.xyz;
  // The world matrix is inside the bones matrixs
    oPos = mul(skinned_Pos, camera_view_proj);
  // Rotate the normal
    oNormal = mul(iNormal, (float3x3) obj_world);
    oTex0 = iTex0;
    oTex1 = iTex1;
  // ------------------------------
  // This needs fixing!!!
  //oTangent  = float4(oNormal,1);
    oTangent.xyz = mul(iTangent.xyz, (float3x3) obj_world);
    oTangent.w = iTangent.w;
}

//-------------------------------------------------------------------------------------

float4 PS(
  float4 Pos : SV_POSITION
, float3 iNormal : NORMAL0
, float4 iTangent : NORMAL1
, float2 iTex0 : TEXCOORD0
, float2 iTex1 : TEXCOORD1
, float3 iWorldPos : TEXCOORD2
) : SV_Target0
{
    float4 o_self_illum = txEmissive.Sample(samLinear, iTex0);
  //o_self_illum.xyz *= self_color;
  // Store in the Alpha channel of the albedo texture, the 'metallic' amount of
  // the material
    float4 o_albedo = txAlbedo.Sample(samLinear, iTex0);
           o_albedo.a = txMetallic.Sample(samLinear, iTex0).r;

    float3 N = computeNormalMap(iNormal, iTangent, iTex0);
  
  // Save roughness in the alpha coord of the N render target
    float roughness = txRoughness.Sample(samLinear, iTex0).r;
    //float4 o_normal = encodeNormal(N, roughness);

    // Si el material lo pide, sobreescribir los valores de la textura
	// por unos escalares uniformes. Only to playtesting...
    //if (scalar_metallic >= 0.f)
    //    o_albedo.a = scalar_metallic;
    //if (scalar_roughness >= 0.f)
    //    o_normal.a = scalar_roughness;

  // Compute the Z in linear space, and normalize it in the range 0...1
  // In the range z=0 to z=zFar of the camera (not zNear)
    float3 camera2wpos = iWorldPos - camera_pos;
    float4 o_depth = dot(camera_front.xyz, camera2wpos) / camera_zfar;

    //---------------------------------------------------------------------------------------------

    // Declare some float3 to store the values from the GBuffer
    float3 wPos, specular_color, reflected_dir, view_dir;
    float4 albedo;

    //decodeGBuffer(iPosition.xy, wPos, N, albedo, specular_color, roughness, reflected_dir, view_dir);

     //-------------------------------------------------------------------------------------------------

   // , out float3 wPos
   //, out  float3 N
   //, out float3 real_albedo
   //, out float3 real_specular_color
   //, out float roughness
   //, out float3 reflected_dir
   //, out float3 view_dir


    //int3 ss_load_coords = uint3(Pos.xy, 0);

  // Recover world position coords
    //    float zlinear = txGBufferLinearDepth.Load(ss_load_coords).x;
    //wPos = getWorldCoords(Pos.xy, zlinear);

  // Recuperar la normal en ese pixel. Sabiendo que se
  // guardó en el rango 0..1 pero las normales se mueven
  // en el rango -1..1
       // float4 N_rt = txNormal.Load(ss_load_coords);
        //N = decodeNormal(N_rt.xyz);
        //N = normalize(N);

  // Get other inputs from the GBuffer
        //albedo = txAlbedo.Load(ss_load_coords);
  // In the alpha of the albedo, we stored the metallic value
  // and in the alpha of the normal, we stored the roughness
        //float metallic = albedo.a;
        //roughness = N_rt.a;
 
  // Apply gamma correction to albedo to bring it back to linear.
       // albedo.rgb = pow(abs(albedo.rgb), 2.2f);

  // Lerp with metallic value to find the good diffuse and specular.
  // If metallic = 0, albedo is the albedo, if metallic = 1, the
  // used albedo is almost black
    //float3 _albedo = albedo.rgb * (1. - metallic);

  // 0.03 default specular value for dielectric.
    specular_color = lerp(0.03f, o_albedo.rgb, o_albedo.a);

  // Eye to object
    float3 incident_dir = normalize(iWorldPos - camera_pos.xyz);
        reflected_dir = normalize(reflect(incident_dir, N));
        view_dir = -incident_dir;

    //-------------------------------------------------------------------------------------------------


  // if roughness = 0 -> I want to use the miplevel 0, the all-detailed image
  // if roughness = 1 -> I will use the most blurred image, the 8-th mipmap, If image was 256x256 => 1x1
    float mipIndex = roughness * roughness * 8.0f; //*32
    float3 env = txEnvironmentMap.SampleLevel(samLinear, reflected_dir, mipIndex).xyz;
  // Convert the color to linear also.
    env = pow(abs(env), 2.2f);

  // The irrandiance, is read using the N direction.
  // Here we are sampling using the cubemap-miplevel 4, and the already blurred txIrradiance texture
  // and mixing it in base to the scalar_irradiance_vs_mipmaps which comes from the ImGui.
  // Remove the interpolation in the final version!!!
    float3 irradiance_mipmaps = txEnvironmentMap.SampleLevel(samLinear, N, 4).xyz; //6
    float3 irradiance_texture = txIrradianceMap.Sample(samLinear, N).xyz;
    float3 irradiance = irradiance_texture * scalar_irradiance_vs_mipmaps + irradiance_mipmaps * (1. - scalar_irradiance_vs_mipmaps);

  // How much the environment we see
    float3 env_fresnel = Specular_F_Roughness(specular_color, 1. - roughness * roughness, N, view_dir);
  //return float4(env_fresnel, 1 );

    float g_ReflectionIntensity = 1.0;
    float g_AmbientLightIntensity = 1.0;

    float ao = txAO.Sample(samLinear, iTex0).x;

    float4 self_illum = txSelfIllum.Load(uint3(Pos.xy, 0));

    // Compute global fog on ambient.
    //float3 pixel_depth = camera_pos.xyz - wPos;
    //float distancet = length(pixel_depth);
    //float visibility = exp(distancet * distancet * -global_fog_density * global_fog_density * 1.442695);
    //visibility = clamp(visibility, 0, 1);

    //float4 final_color = float4(env_fresnel * env * g_ReflectionIntensity + albedo.xyz * irradiance * g_AmbientLightIntensity, 1.0f);
    //final_color = final_color * global_ambient_adjustment * ao;
    //return lerp(float4(env, 1), final_color, visibility) + float4(self_illum.xyz, 1) * global_ambient_adjustment * global_self_intensity;

    float4 final_color = float4(/*env_fresnel * env **/ g_ReflectionIntensity +
                              o_albedo.xyz /** irradiance */* g_AmbientLightIntensity
                              , 1.0f) /*+ o_self_illum*/;


    final_color = final_color * global_ambient_adjustment * ao;
    //final_color = lerp(float4(env, 1), final_color, 1) + float4(self_illum.xyz, 1) * global_ambient_adjustment;

    final_color.a = 1;

    float intensity;
    if (light_point == 1)
    {
        intensity = dot(normalize(Pos.xyz - light_pos), N);
    }
    else
    {
        intensity = dot(normalize(light_direction.xyz), N);
    }
 
    // Discretize the intensity, based on a few cutoff points
    if (intensity > 0.8)
        final_color = float4(1.0, 1.0, 1.0, 1.0) * o_albedo;
    else if (intensity < -0.8)
        final_color = float4(0.35, 0.35, 0.35, 1.0) * o_albedo;
    else
        final_color = float4(0.7, 0.7, 0.7, 1.0) * o_albedo;

    return final_color;
}


//--------------------------------------------------------------------------------------
void decodeGBuffer(
     in float2 iPosition // Screen coords
   , out float3 wPos
   , out float3 N
   , out float3 real_albedo
   , out float3 real_specular_color
   , out float roughness
   , out float3 reflected_dir
   , out float3 view_dir
   )
{

    int3 ss_load_coords = uint3(iPosition.xy, 0);

  // Recover world position coords
    float zlinear = txGBufferLinearDepth.Load(ss_load_coords).x;
    wPos = getWorldCoords(iPosition.xy, zlinear);

  // Recuperar la normal en ese pixel. Sabiendo que se
  // guardó en el rango 0..1 pero las normales se mueven
  // en el rango -1..1
    float4 N_rt = txGBufferNormals.Load(ss_load_coords);
    N = decodeNormal(N_rt.xyz);
    N = normalize(N);

  // Get other inputs from the GBuffer
    float4 albedo = txGBufferAlbedos.Load(ss_load_coords);
  // In the alpha of the albedo, we stored the metallic value
  // and in the alpha of the normal, we stored the roughness
    float metallic = albedo.a;
    roughness = N_rt.a;
 
  // Apply gamma correction to albedo to bring it back to linear.
    albedo.rgb = pow(abs(albedo.rgb), 2.2f);

  // Lerp with metallic value to find the good diffuse and specular.
  // If metallic = 0, albedo is the albedo, if metallic = 1, the
  // used albedo is almost black
    real_albedo = albedo.rgb * (1. - metallic);

  // 0.03 default specular value for dielectric.
    real_specular_color = lerp(0.03f, albedo.rgb, metallic);

  // Eye to object
    float3 incident_dir = normalize(wPos - camera_pos.xyz);
    reflected_dir = normalize(reflect(incident_dir, N));
    view_dir = -incident_dir;
}

float NormalDistribution_GGX(float a, float NdH)
{
    // Isotropic ggx.
    float a2 = a * a;
    float NdH2 = NdH * NdH;

    float denominator = NdH2 * (a2 - 1.0f) + 1.0f;
    denominator *= denominator;
    denominator *= PI;

    return a2 / denominator;
}

float Geometric_Smith_Schlick_GGX(float a, float NdV, float NdL)
{
        // Smith schlick-GGX.
    float k = a * 0.5f;
    float GV = NdV / (NdV * (1 - k) + k);
    float GL = NdL / (NdL * (1 - k) + k);

    return GV * GL;
}


float Specular_D(float a, float NdH)
{
    return NormalDistribution_GGX(a, NdH);
}

float Specular_G(float a, float NdV, float NdL, float NdH, float VdH, float LdV)
{
    return Geometric_Smith_Schlick_GGX(a, NdV, NdL);
}

float3 Fresnel_Schlick(float3 specularColor, float3 h, float3 v)
{
    return (specularColor + (1.0f - specularColor) * pow((1.0f - saturate(dot(v, h))), 5));
}

float3 Specular_F(float3 specularColor, float3 h, float3 v)
{
    return Fresnel_Schlick(specularColor, h, v);
}

float3 Specular(float3 specularColor, float3 h, float3 v, float3 l, float a, float NdL, float NdV, float NdH, float VdH, float LdV)
{
    return ((Specular_D(a, NdH) * Specular_G(a, NdV, NdL, NdH, VdH, LdV)) * Specular_F(specularColor, v, h)) / (4.0f * NdL * NdV + 0.0001f);
}

//--------------------------------------------------------------------------------------
// Ambient pass, to compute the ambient light of each pixel
float4 PS_ambient(
  in float4 iPosition : SV_Position
, in float2 iUV : TEXCOORD0
) : SV_Target
{

  // Declare some float3 to store the values from the GBuffer
    float3 wPos, N, albedo, specular_color, reflected_dir, view_dir;
    float roughness;
    decodeGBuffer(iPosition.xy, wPos, N, albedo, specular_color, roughness, reflected_dir, view_dir);

  // if roughness = 0 -> I want to use the miplevel 0, the all-detailed image
  // if roughness = 1 -> I will use the most blurred image, the 8-th mipmap, If image was 256x256 => 1x1
    float mipIndex = roughness * roughness * 8.0f; //*32
    float3 env = txEnvironmentMap.SampleLevel(samLinear, reflected_dir, mipIndex).xyz;
  // Convert the color to linear also.
    env = pow(abs(env), 2.2f);

  // The irrandiance, is read using the N direction.
  // Here we are sampling using the cubemap-miplevel 4, and the already blurred txIrradiance texture
  // and mixing it in base to the scalar_irradiance_vs_mipmaps which comes from the ImGui.
  // Remove the interpolation in the final version!!!
    float3 irradiance_mipmaps = txEnvironmentMap.SampleLevel(samLinear, N, 4).xyz; //6
    float3 irradiance_texture = txIrradianceMap.Sample(samLinear, N).xyz;
    float3 irradiance = irradiance_texture * scalar_irradiance_vs_mipmaps + irradiance_mipmaps * (1. - scalar_irradiance_vs_mipmaps);

  // How much the environment we see
    float3 env_fresnel = Specular_F_Roughness(specular_color, 1. - roughness * roughness, N, view_dir);
  //return float4(env_fresnel, 1 );

    float g_ReflectionIntensity = 1.0;
    float g_AmbientLightIntensity = 1.0;

    float ao = txAO.Sample(samLinear, iUV).x;

    float4 self_illum = txSelfIllum.Load(uint3(iPosition.xy, 0));

    // Compute global fog on ambient.
    //float3 pixel_depth = camera_pos.xyz - wPos;
    //float distancet = length(pixel_depth);
    //float visibility = exp(distancet * distancet * -global_fog_density * global_fog_density * 1.442695);
    //visibility = clamp(visibility, 0, 1);

    //float4 final_color = float4(env_fresnel * env * g_ReflectionIntensity + albedo.xyz * irradiance * g_AmbientLightIntensity, 1.0f);
    //final_color = final_color * global_ambient_adjustment * ao;
    //return lerp(float4(env, 1), final_color, visibility) + float4(self_illum.xyz, 1) * global_ambient_adjustment * global_self_intensity;

    float4 final_color = float4(env_fresnel * env * g_ReflectionIntensity +
                              albedo.xyz * irradiance * g_AmbientLightIntensity
                              , 1.0f) + self_illum;


    final_color = final_color * global_ambient_adjustment * ao;
    return lerp(float4(env, 1), final_color, 1) + float4(self_illum.xyz, 1) * global_ambient_adjustment;

}





