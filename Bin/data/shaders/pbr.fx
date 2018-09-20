#include "common.fx"

#define PI 3.14159265359f


//--------------------------------------------------------------------------------------
// GBuffer generation pass. Vertex
//--------------------------------------------------------------------------------------
void VS_GBuffer(
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

//--------------------------------------------------------------------------------------
// GBufferSkin generation pass. Vertex
//--------------------------------------------------------------------------------------
void VS_GBufferSkin(
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

//--------------------------------------------------------------------------------------
// GBuffer Catarata generation pass. Vertex
//--------------------------------------------------------------------------------------
void VS_GBuffer_Catarata(
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
    oTex0 = float2(iTex0.x, (iTex0.y - global_world_time * 0.2));
    oTex1 = iTex1;
    oWorldPos = world_pos.xyz;
}

//--------------------------------------------------------------------------------------
// GBuffer generation pass. Pixel shader
//--------------------------------------------------------------------------------------
void PS_GBuffer(
  float4 Pos : SV_POSITION
, float3 iNormal : NORMAL0
, float4 iTangent : NORMAL1
, float2 iTex0 : TEXCOORD0
, float2 iTex1 : TEXCOORD1
, float3 iWorldPos : TEXCOORD2
, out float4 o_albedo : SV_Target0
, out float4 o_normal : SV_Target1
, out float1 o_depth : SV_Target2
, out float4 o_self_illum : SV_Target3
, out float1 o_cell : SV_Target5
, out float4 o_sublime : SV_Target6
)
{
    o_cell = (txCell.Sample(samLinear, iTex0)).x;
    o_self_illum = txEmissive.Sample(samLinear, iTex0);
  //o_self_illum.xyz *= self_color;
  // Store in the Alpha channel of the albedo texture, the 'metallic' amount of
  // the material
    o_albedo = txAlbedo.Sample(samLinear, iTex0);
    //o_albedo.a = txMetallic.Sample(samLinear, iTex0).r;

    float3 N = computeNormalMap(iNormal, iTangent, iTex0);
  
  // Save roughness in the alpha coord of the N render target
    float roughness = txSublime.Sample(samLinear, iTex0).g; //txRoughness.Sample(samLinear, iTex0).r;
    o_normal = encodeNormal(N, roughness);

    // Si el material lo pide, sobreescribir los valores de la textura
	// por unos escalares uniformes. Only to playtesting...
    //if (scalar_metallic >= 0.f)
    //    o_albedo.a = scalar_metallic;
    //if (scalar_roughness >= 0.f)
    //    o_normal.a = scalar_roughness;

    o_sublime = txSublime.Sample(samLinear, iTex0);

  // Compute the Z in linear space, and normalize it in the range 0...1
  // In the range z=0 to z=zFar of the camera (not zNear)
    float3 camera2wpos = iWorldPos - camera_pos;
    o_depth = dot(camera_front.xyz, camera2wpos) / camera_zfar;
}

//--------------------------------------------------------------------------------------
// GBuffer Catarata generation pass. Pixel shader
//--------------------------------------------------------------------------------------
void PS_GBuffer_Catarata(
  float4 Pos : SV_POSITION
, float3 iNormal : NORMAL0
, float4 iTangent : NORMAL1
, float2 iTex0 : TEXCOORD0
, float2 iTex1 : TEXCOORD1
, float3 iWorldPos : TEXCOORD2
, out float4 o_albedo : SV_Target0
, out float4 o_normal : SV_Target1
, out float1 o_depth : SV_Target2
, out float4 o_self_illum : SV_Target3
, out float1 o_cell : SV_Target5
)
{
    o_cell = (txCell.Sample(samLinear, iTex0)).x;
    o_self_illum = txEmissive.Sample(samLinear, iTex0);
  //o_self_illum.xyz *= self_color;
  // Store in the Alpha channel of the albedo texture, the 'metallic' amount of
  // the material
    o_albedo = txAlbedo.Sample(samLinear, iTex0);
    float4 alpha_color = txAlpha.Sample(samLinear, iTex0);
    o_albedo.a = alpha_color.r;

    if (o_albedo.a < 0.3) 
        discard;

    float3 N = computeNormalMap(iNormal, iTangent, iTex0);
  
  // Save roughness in the alpha coord of the N render target
    float roughness = txSublime.Sample(samLinear, iTex0).g; //txRoughness.Sample(samLinear, iTex0).r;
    o_normal = encodeNormal(N, roughness);

    // Si el material lo pide, sobreescribir los valores de la textura
	// por unos escalares uniformes. Only to playtesting...
    //if (scalar_metallic >= 0.f)
    //    o_albedo.a = scalar_metallic;
    //if (scalar_roughness >= 0.f)
    //    o_normal.a = scalar_roughness;

  // Compute the Z in linear space, and normalize it in the range 0...1
  // In the range z=0 to z=zFar of the camera (not zNear)
    float3 camera2wpos = iWorldPos - camera_pos;
    o_depth = dot(camera_front.xyz, camera2wpos) / camera_zfar;
}

//--------------------------------------------------------------------------------------
// GBuffer Alpha generation pass. Pixel shader
//--------------------------------------------------------------------------------------
void PS_GBuffer_Alpha(
  float4 Pos : SV_POSITION
, float3 iNormal : NORMAL0
, float4 iTangent : NORMAL1
, float2 iTex0 : TEXCOORD0
, float2 iTex1 : TEXCOORD1
, float3 iWorldPos : TEXCOORD2
, out float4 o_albedo : SV_Target0
, out float4 o_normal : SV_Target1
, out float1 o_depth : SV_Target2
, out float4 o_self_illum : SV_Target3
, out float1 o_cell : SV_Target5
)
{
    o_cell = (txCell.Sample(samLinear, iTex0)).x;
    o_self_illum = txEmissive.Sample(samLinear, iTex0);
  //o_self_illum.xyz *= self_color;
  // Store in the Alpha channel of the albedo texture, the 'metallic' amount of
  // the material
    o_albedo = txAlbedo.Sample(samLinear, iTex0);

    if (txAlpha.Sample(samLinear, iTex0).r < 0.3) 
        discard;

    float3 N = computeNormalMap(iNormal, iTangent, iTex0);
  
  // Save roughness in the alpha coord of the N render target
    float roughness = txSublime.Sample(samLinear, iTex0).g; //txRoughness.Sample(samLinear, iTex0).r;
    o_normal = encodeNormal(N, roughness);

    // Si el material lo pide, sobreescribir los valores de la textura
	// por unos escalares uniformes. Only to playtesting...
    //if (scalar_metallic >= 0.f)
    //    o_albedo.a = scalar_metallic;
    //if (scalar_roughness >= 0.f)
    //    o_normal.a = scalar_roughness;

  // Compute the Z in linear space, and normalize it in the range 0...1
  // In the range z=0 to z=zFar of the camera (not zNear)
    float3 camera2wpos = iWorldPos - camera_pos;
    o_depth = dot(camera_front.xyz, camera2wpos) / camera_zfar;
}

//--------------------------------------------------------------------------------------
// GBuffer Viento generation pass. Pixel shader
//--------------------------------------------------------------------------------------
void PS_GBuffer_Viento(
  float4 Pos : SV_POSITION
, float3 iNormal : NORMAL0
, float4 iTangent : NORMAL1
, float2 iTex0 : TEXCOORD0
, float2 iTex1 : TEXCOORD1
, float3 iWorldPos : TEXCOORD2
, out float4 o_albedo : SV_Target0
, out float4 o_normal : SV_Target1
, out float1 o_depth : SV_Target2
, out float4 o_self_illum : SV_Target3
, out float1 o_cell : SV_Target5
)
{
    //o_cell = (txCell.Sample(samLinear, iTex0)).x;
    o_self_illum = txEmissive.Sample(samLinear, iTex0);
  //o_self_illum.xyz *= self_color;
  // Store in the Alpha channel of the albedo texture, the 'metallic' amount of
  // the material
    o_albedo = txAlbedo.Sample(samLinear, iTex0);
    o_albedo.a = txAlpha.Sample(samLinear, iTex0).r;

    //if (o_albedo.a < 0.3) 
    //    discard;

    float3 N = computeNormalMap(iNormal, iTangent, iTex0);
  
  // Save roughness in the alpha coord of the N render target
    float roughness = 1.0f; //txRoughness.Sample(samLinear, iTex0).r;
    o_normal = encodeNormal(N, roughness);

    // Si el material lo pide, sobreescribir los valores de la textura
	// por unos escalares uniformes. Only to playtesting...
    //if (scalar_metallic >= 0.f)
    //    o_albedo.a = scalar_metallic;
    //if (scalar_roughness >= 0.f)
    //    o_normal.a = scalar_roughness;

  // Compute the Z in linear space, and normalize it in the range 0...1
  // In the range z=0 to z=zFar of the camera (not zNear)
    float3 camera2wpos = iWorldPos - camera_pos;
    o_depth = dot(camera_front.xyz, camera2wpos) / camera_zfar;
}

// -------------------------------------------------
// Gloss = 1 - rough*rough

void PS_GBuffer_Parallax(
  float4 Pos : SV_POSITION
  , float3 iNormal : NORMAL0
  , float4 iTangent : NORMAL1
  , float2 iTex0 : TEXCOORD0
  , float2 iTex1 : TEXCOORD1
  , float3 iWorldPos : TEXCOORD2
  , float3 oCameraOut : TEXCOORD3
  , out float4 o_albedo : SV_Target0
  , out float4 o_normal : SV_Target1
  , out float1 o_depth : SV_Target2
  , out float4 o_selfIllum : SV_Target3
)
{
  // Store in the Alpha channel of the albedo texture, the 'metallic' amount of
  // the material
    float3x3 TBN = computeTBN(iNormal, iTangent);
    TBN = transpose(TBN);
    float3 view_dir = normalize(mul(camera_pos, TBN) - mul(iWorldPos, TBN));
    iTex0 = parallaxMapping(iTex0, view_dir);

    o_albedo = txAlbedo.Sample(samLinear, iTex0);
    //o_albedo.a = txMetallic.Sample(samLinear, iTex0).r;
    o_selfIllum = txEmissive.Sample(samLinear, iTex0) * 1; // self_intensity;
    //o_selfIllum.xyz *= self_color;

  // Save roughness in the alpha coord of the N render target
    float roughness = txSublime.Sample(samLinear, iTex0).g;
    float3 N = computeNormalMap(iNormal, iTangent, iTex0);
    o_normal = encodeNormal(N, roughness);

  // Si el material lo pide, sobreescribir los valores de la textura
  // por unos escalares uniformes. Only to playtesting...
    //if (scalar_metallic >= 0.f)
    //    o_albedo.a = scalar_metallic;
    //if (scalar_roughness >= 0.f)
    //    o_normal.a = scalar_roughness;

  // Compute the Z in linear space, and normalize it in the range 0...1
  // In the range z=0 to z=zFar of the camera (not zNear)
    float3 camera2wpos = iWorldPos - camera_pos;
    o_depth = dot(camera_front.xyz, camera2wpos) / camera_zfar;
}

//--------------------------------------------------------------------------------------
void PS_GBufferMix(
  float4 Pos : SV_POSITION
, float3 iNormal : NORMAL0
, float4 iTangent : NORMAL1
, float2 iTex0 : TEXCOORD0
, float2 iTex1 : TEXCOORD1
, float3 iWorldPos : TEXCOORD2
, out float4 o_albedo : SV_Target0
, out float4 o_normal : SV_Target1
, out float1 o_depth : SV_Target2
)
{
  // This is different -----------------------------------------
  // Using second set of texture coords
    float4 weight_texture_boost = txMixBlendWeights.Sample(samLinear, iTex1);

    float4 albedoR = txAlbedo.Sample(samLinear, iTex0);
    float4 albedoG = txAlbedo1.Sample(samLinear, iTex0);
    float4 albedoB = txAlbedo2.Sample(samLinear, iTex0);

  // Use the alpha of the albedo as heights + texture blending extra weights + material ctes extra weights (imgui)
    float w1, w2, w3;
    computeBlendWeights(albedoR.a + mix_boost_r + weight_texture_boost.r
                     , albedoG.a + mix_boost_g + weight_texture_boost.g
                     , albedoB.a + mix_boost_b + weight_texture_boost.b
                     , w1, w2, w3);

  // Use the weight to 'blend' the albedo colors
    float4 albedo = albedoR * w1 + albedoG * w2 + albedoB * w3;
    o_albedo.xyz = albedo.xyz;

  // Mix the normal
    float3 normalR = txNormal.Sample(samLinear, iTex0).xyz * 2.0 - 1.0;
    float3 normalG = txNormal1.Sample(samLinear, iTex0).xyz * 2.0 - 1.0;
    float3 normalB = txNormal2.Sample(samLinear, iTex0).xyz * 2.0 - 1.0;
    float3 normal_color = normalR * w1 + normalG * w2 + normalB * w3;
    float3x3 TBN = computeTBN(iNormal, iTangent);

  // Normal map comes in the range 0..1. Recover it in the range -1..1
    float3 wN = mul(normal_color, TBN);
    float3 N = normalize(wN);

  // Missing: Do the same with the metallic & roughness channels
  // ...

  // Possible plain blending without heights
  //o_albedo.xyz = lerp( albedoB.xyz, albedoG.xyz, weight_texture_boost.y );
  //o_albedo.xyz = lerp( o_albedo.xyz, albedoR.xyz, weight_texture_boost.x );

  //o_albedo.xyz = float3( iTex1.xy, 0 );		// Show the texture coords1

  //o_albedo.xyz = weight_texture_boost.xyz;	// Show the extra weight textures

    //o_albedo.a = txMetallic.Sample(samLinear, iTex0).r;

  // This is the same -----------------------------------------
  // Save roughness in the alpha coord of the N render target
    float roughness = txRoughness.Sample(samLinear, iTex0).r;
    o_normal = encodeNormal(N, roughness);

  // Compute the Z in linear space, and normalize it in the range 0...1
  // In the range z=0 to z=zFar of the camera (not zNear)
    float3 camera2wpos = iWorldPos - camera_pos;
    o_depth = dot(camera_front.xyz, camera2wpos) / camera_zfar;
}


//--------------------------------------------------------------------------------------
void decodeGBuffer(
     in float2 iPosition // Screen coords
   , out float3 wPos
   , out float3 N
   , out float4 real_albedo
   , out float3 real_specular_color
   , out float roughness
   , out float3 reflected_dir
   , out float3 view_dir
   , out bool cell_shading
   , out float ao
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

    float4 sublime = txGBufferSublime.Load(ss_load_coords);

    //R = Oclussion
    //G = Roughness
    //B = Metallic


  // Get other inputs from the GBuffer
    float4 albedo = txGBufferAlbedos.Load(ss_load_coords);
  // In the alpha of the albedo, we stored the metallic value
  // and in the alpha of the normal, we stored the roughness
    //sublime.rgb = pow(abs(sublime.rgb), 2.2f);
    
    float metallic = sublime.b;
    roughness = sublime.g;
    ao = sublime.r;
 
  // Apply gamma correction to albedo to bring it back to linear.
    albedo.rgb = pow(abs(albedo.rgb), 2.2f);

  // Lerp with metallic value to find the good diffuse and specular.
  // If metallic = 0, albedo is the albedo, if metallic = 1, the
  // used albedo is almost black
    real_albedo = float4(albedo.rgb * (1. - metallic), txGBufferAlpha.Load(ss_load_coords).r);
    //real_albedo.a = txGBufferAlpha.Load(ss_load_coords);

  // 0.03 default specular value for dielectric.
    real_specular_color = lerp(0.03f, real_albedo.rgb, metallic);

  // Eye to object
    float3 incident_dir = normalize(wPos - camera_pos.xyz);
    reflected_dir = normalize(reflect(incident_dir, N));
    view_dir = -incident_dir;

  // Comprobamos si el objeto se pinta con PBR o Cell Shading
    if ((txGBufferCell.Load(ss_load_coords)).x == 0.f)
        cell_shading = false;
    else cell_shading = true;
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
    float4 albedo;
    float3 wPos, N, specular_color, reflected_dir, view_dir;
    float roughness, ao;
    bool cell;
    decodeGBuffer(iPosition.xy, wPos, N, albedo, specular_color, roughness, reflected_dir, view_dir, cell, ao);

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

     ao = txAO.Sample(samLinear, iUV).x;

    float4 self_illum = txSelfIllum.Load(uint3(iPosition.xy, 0));

    // Compute global fog on ambient.
    //float3 pixel_depth = camera_pos.xyz - wPos;
    //float distancet = length(pixel_depth);
    //float visibility = exp(distancet * distancet * -global_fog_density * global_fog_density * 1.442695);
    //visibility = clamp(visibility, 0, 1);

    //float4 final_color = float4(env_fresnel * env * g_ReflectionIntensity + albedo.xyz * irradiance * g_AmbientLightIntensity, 1.0f);
    //final_color = final_color * global_ambient_adjustment * ao;
    //return lerp(float4(env, 1), final_color, visibility) + float4(self_illum.xyz, 1) * global_ambient_adjustment * global_self_intensity;

    // From wPos to Light
    float3 light_dir_full = light_pos.xyz - wPos;
    float distance_to_light = length(light_dir_full);
    float3 light_dir = light_dir_full / distance_to_light;

    float4 final_color;

    if (cell)
    {
        final_color = float4( /*env_fresnel * env * g_ReflectionIntensity +*/
                              albedo.xyz * irradiance * g_AmbientLightIntensity
                              , albedo.a) + self_illum;


        final_color = final_color * 5/* * ao*/;
        final_color = lerp(float4(env, 1), final_color, 1) + float4(self_illum.xyz, 1) * 5;

        final_color.a = 1;

        float intensity = dot(normalize(light_dir), N);
        
 
    // Discretize the intensity, based on a few cutoff points
        if (intensity > 0.8)
            final_color = float4(1.0, 1.0, 1.0, 1.0) * final_color;//float4(albedo, 1);
        else if (intensity < -0.8)
            final_color = float4(0.35, 0.35, 0.35, 1.0) * final_color; //float4(albedo, 1);
        else
            final_color = float4(0.7, 0.7, 0.7, 1.0) * final_color; //float4(albedo, 1);
    }
    else
    {
        final_color = float4(env_fresnel * env * g_ReflectionIntensity +
                              albedo.xyz * irradiance * g_AmbientLightIntensity
                              , albedo.a) + self_illum;


        final_color = final_color * global_ambient_adjustment * ao;
        final_color = lerp(float4(env, 1), final_color, 1) + float4(self_illum.xyz, 1) * global_ambient_adjustment;

    }
  
   
    return final_color;

}

//--------------------------------------------------------------------------------------
// The geometry that approximates the light volume uses this shader
void VS_pass(
  in float4 iPos : POSITION
, in float3 iNormal : NORMAL0
, in float2 iTex0 : TEXCOORD0
, in float2 iTex1 : TEXCOORD0
, in float4 iTangent : NORMAL1
, out float4 oPos : SV_POSITION
)
{
    float4 world_pos = mul(iPos, obj_world);
    oPos = mul(world_pos, camera_view_proj);
}

// --------------------------------------------------------
float3 Diffuse(float3 pAlbedo)
{
    return pAlbedo / PI;
}

// --------------------------------------------------------
float4 shade(
  float4 iPosition
, out float3 light_dir
, bool use_shadows
, bool use_cube_shadows
)
{
  // Decode GBuffer information
    float4 albedo;
    float3 wPos, N, specular_color, reflected_dir, view_dir;
    float roughness, ao;
    bool cell;
    decodeGBuffer(iPosition.xy, wPos, N, albedo, specular_color, roughness, reflected_dir, view_dir, cell, ao);
    N = normalize(N);
  // Shadow factor entre 0 (totalmente en sombra) y 1 (no ocluido)
    float shadow_factor = use_shadows ? computeShadowFactor(wPos) : 1.; // shadow factor

  // From wPos to Light
    float3 light_dir_full = light_pos.xyz - wPos;
    float distance_to_light = length(light_dir_full);
    light_dir = light_dir_full / distance_to_light;

    // Cube uses the light dir to access the shadowMap, not the wPos
    shadow_factor *= use_cube_shadows ? computeCubeShadowFactor(light_dir_full) : 1.;

    float NdL = saturate(dot(N, light_dir));
    float NdV = saturate(dot(N, view_dir));
    float3 h = normalize(light_dir + view_dir); // half vector

    float NdH = saturate(dot(N, h));
    float VdH = saturate(dot(view_dir, h));
    float LdV = saturate(dot(light_dir, view_dir));
    float a = max(0.001f, roughness * roughness);
    float3 cDiff = Diffuse(albedo.xyz);
    float3 cSpec = Specular(specular_color, h, view_dir, light_dir, a, NdL, NdV, NdH, VdH, LdV);

    float att = (1. - smoothstep(0.90, 0.98, distance_to_light / light_radius));
  // att *= 1 / distance_to_light;
   

    float4 final_color2;

    if (cell)
    {
        half NdotL = dot(normalize(light_dir), N);
        if (NdotL <= 0.0)
            NdotL = 0;
        else
            NdotL = 1;
        float3 final_color = light_color.xyz * cDiff /** NdL*/ /** (cDiff * (1.0f - cSpec) + cSpec) **/* att * light_intensity * shadow_factor;

        final_color2 = float4(final_color, 1);

        final_color2.a = 1;

        float intensity= dot(normalize(light_dir), N);
 
    // Discretize the intensity, based on a few cutoff points
        if (intensity > 0.8)
            final_color2 = float4(1.0, 1.0, 1.0, 1.0) * final_color2;
        else if (intensity < -0.8)
            final_color2 = float4(0.35, 0.35, 0.35, 1.0) * final_color2;
        else
            final_color2 = float4(0.7, 0.7, 0.7, 1.0) * final_color2;
    }
    else
    {
        float3 final_color = light_color.xyz * NdL * (cDiff * (1.0f - cSpec) + cSpec) * att * light_intensity * shadow_factor;

        final_color2 = float4(final_color, 1);
    }

    return final_color2;
}

float4 PS_point_lights(in float4 iPosition : SV_Position) : SV_Target
{
    float3 out_lightdir;
    return shade(iPosition, out_lightdir, false, false);
}

float4 PS_point_lights_shadow(in float4 iPosition : SV_Position) : SV_Target
{
    float3 out_lightdir;
    return shade(iPosition, out_lightdir, false, true);
}

float4 PS_dir_lights(in float4 iPosition : SV_Position) : SV_Target
{
    float3 out_lightdir;
    return shade(iPosition, out_lightdir, true, false);
}

float4 PS_spot_lights(in float4 iPosition : SV_Position) : SV_Target
{
    float3 out_lightdir;
    float4 light_color = shade(iPosition, out_lightdir, true, false);

    float theta = dot(out_lightdir, -light_direction.xyz);
    float att_spot = clamp((theta - light_outer_cut) / (light_inner_cut - light_outer_cut), 0, 1);
    float clamp_spot = theta > light_angle ? 1.0 * att_spot : 0.0; // spot factor 
	
    return light_color * clamp_spot;
}


// ----------------------------------------
void VS_skybox(
  in float4 iPosition : POSITION
, in float4 iColor : COLOR0
, out float4 oPosition : SV_Position
)
{
  // Convert the range 0..1 from iPosition to -1..1 to match the homo space
    oPosition = float4(iPosition.x * 2 - 1., 1 - iPosition.y * 2, 1, 1);
}

// --------------------------------------------------------
float4 PS_skybox(in float4 iPosition : SV_Position) : SV_Target
{
    float3 view_dir = mul(float4(iPosition.xy, 1, 1), camera_screen_to_world).xyz;
    float4 skybox_color = txEnvironmentMap.Sample(samLinear, view_dir);
    return float4(skybox_color.xyz, 1) * global_ambient_adjustment;
}
