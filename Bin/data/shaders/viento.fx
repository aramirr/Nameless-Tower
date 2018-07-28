#include "common.fx"

//--------------------------------------------------------------------------------------
// Alpha generation pass. Vertex
//--------------------------------------------------------------------------------------
void VS_Viento(
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
    oTex0 = float2(iTex0.x, (iTex0.y + global_world_time * 1.6));
    oTex1 = iTex1;
    oWorldPos = world_pos.xyz;
 //   float4 world_pos = mul(iPos, obj_world);

	//// Rotar la normal segun la transform del objeto
 //   oNormal = mul(iNormal, (float3x3) obj_world);
 //   oTangent.xyz = mul(iTangent.xyz, (float3x3) obj_world);
 //   oTangent.w = iTangent.w;

 //   oPos = mul(world_pos, camera_view_proj);

	//// Las uv's se pasan directamente al ps
 //   oTex0 = iTex0;
 //   oTex1 = iTex1;
 //   oWorldPos = world_pos.xyz;
}

//--------------------------------------------------------------------------------------
// Alpha generation pass. Pixel shader
//--------------------------------------------------------------------------------------
float4 PS_Viento(
  float4 Pos : SV_POSITION
, float3 iNormal : NORMAL0
, float4 iTangent : NORMAL1
, float2 iTex0 : TEXCOORD0
, float2 iTex1 : TEXCOORD1
, float3 iWorldPos : TEXCOORD2
) : SV_Target0
{
    //int3 ss_load_coords = uint3(Pos.xy, 0);

  //// Convert world coords to camera space in the range 0..1 to access the rt_main texture
  //  float4 pos_proj_space = mul(float4(iWorldPos, 1), camera_view_proj);
  //  float3 pos_homo_space = pos_proj_space.xyz / pos_proj_space.w; // -1..1
  //  float2 pos_camera_unit_space = float2((1 + pos_homo_space.x) * 0.5, (1 - pos_homo_space.y) * 0.5);
    float4 final_color = txAlbedo.Sample(samLinear, iTex0);
    float4 alpha_color = txAlpha.Sample(samLinear, iTex0); // * txAlpha.Sample(samLinear, iTex0) + float4(0, 0, 0, 0) * (1. - txAlpha.Sample(samLinear, iTex0)); /** (1. - txTransparency.Sample(samLinear, iTex0))*/; //obj_color.a;

  //  float3 eye = normalize(iWorldPos - camera_pos);

  //// Apply a small fresnel effect
  //  float amount = dot(iNormal, -eye);
  //  amount = saturate(amount);
  //// Change the curve of the influence,.
  //  amount = pow(amount, 0.5);

  //  float3 irradiance_mipmaps = txEnvironmentMap.SampleLevel(samLinear, iNormal, 6).xyz;
  //  float3 irradiance_texture = txIrradianceMap.Sample(samLinear, iNormal).xyz;
  //  float3 irradiance = irradiance_texture * scalar_irradiance_vs_mipmaps + irradiance_mipmaps * (1. - scalar_irradiance_vs_mipmaps);

  //  float g_ReflectionIntensity = 1.0;
  //  float g_AmbientLightIntensity = 1.0;

  //  float4 self_illum = float4(0, 0, 0, 0); //txGSelfIllum.Load(uint3(iPosition.xy,0));

  //  float roughness = 1.f; //length(txRoughness.Sample(samLinear, iTex0));
  //  float3 env_fresnel = Specular_F_Roughness(color_base, 1. - roughness * roughness, iNormal, eye);

  //  float4 final_color = float4(env_fresnel /** amount*/ * g_ReflectionIntensity +
	 // color_base.xyz * irradiance * g_AmbientLightIntensity
	 // , 1.0f) + self_illum;
    //float4 alpha = txAlpha.Sample(samLinear, iTex0);
    //if (alpha.x == 0)
    //{
    //    final_color *= txAlpha.Sample(samLinear, iTex0); //obj_color.a;
    //    return final_color;
    //}
    //final_color = float4(final_color.xyz, 1.0f);
    final_color.a = alpha_color.r;
    if (final_color.a < 0.3) 
        discard;
    return final_color;
 // float4 o_self_illum = txEmissive.Sample(samLinear, iTex0);


 // //o_self_illum.xyz *= self_color;
 // // Store in the Alpha channel of the albedo texture, the 'metallic' amount of
 // // the material

 // float4 o_albedo = txAlbedo.Sample(samLinear, iTex0);
 // float4 o_transparency = txTransparency.Sample(samLinear, iTex0);

 // //o_albedo.a = 0.2f;// txMetallic.Sample(samLinear, iTex0).r;

 // float3 N = computeNormalMap( iNormal, iTangent, iTex0 );
  
 // // Save roughness in the alpha coord of the N render target
 // float roughness = txRoughness.Sample(samLinear, iTex0).r;
 // float4 o_normal = encodeNormal( N , roughness );

 // // REMOVE ALL THIS
 // // Si el material lo pide, sobreescribir los valores de la textura
 // // por unos escalares uniformes. Only to playtesting...
 // //if (scalar_metallic >= 0.f)
 // //  o_albedo.a = scalar_metallic;
 // //if (scalar_roughness >= 0.f)
 // //  o_normal.a = scalar_roughness;

 // // Compute the Z in linear space, and normalize it in the range 0...1
 // // In the range z=0 to z=zFar of the camera (not zNear)
 // float3 camera2wpos = iWorldPos - camera_pos;
 // float4 o_depth = dot( camera_front.xyz, camera2wpos ) / camera_zfar;

 // //--------------------------------------------------------------------------------

 // //float3 wPos, albedo, specular_color, reflected_dir, view_dir; //+N

 // //int3 ss_load_coords = uint3(Pos.xy, 0);

 // // Recover world position coords
 // //float  zlinear = o_depth.Load(ss_load_coords).x;
 // //wPos = getWorldCoords(iPosition.xy, zlinear);

 // // Recuperar la normal en ese pixel. Sabiendo que se
 // // guardó en el rango 0..1 pero las normales se mueven
 // // en el rango -1..1
 // //float4 N_rt = txGBufferNormals.Load(ss_load_coords);
 // N = decodeNormal( o_normal.xyz );
 // N = normalize( N );

 // // Get other inputs from the GBuffer
 // //float4 albedo = txGBufferAlbedos.Load(ss_load_coords);
 // // In the alpha of the albedo, we stored the metallic value
 // // and in the alpha of the normal, we stored the roughness
 // float  metallic = o_albedo.a;
 //        roughness = o_normal.a;
 
 // // Apply gamma correction to albedo to bring it back to linear.
 // o_albedo.rgb = pow(abs(o_albedo.rgb), 2.2f);
 // o_transparency.rgba = pow(abs(o_transparency.rgba), 2.2f);

 // // Lerp with metallic value to find the good diffuse and specular.
 // // If metallic = 0, albedo is the albedo, if metallic = 1, the
 // // used albedo is almost black
 //   //float4 albedo = o_albedo.rgba * o_transparency.rgba;

 // // 0.03 default specular value for dielectric.
 // float3 specular_color = lerp(0.03f, o_albedo.rgb, metallic);

 // // Eye to object
 // float3 incident_dir = normalize(iWorldPos - camera_pos.xyz);
 // float3 reflected_dir = normalize(reflect(incident_dir, N));
 // float3 view_dir = -incident_dir;
 // //--------------------------------------------------------------------------

 // // Declare some float3 to store the values from the GBuffer
 // //float3 wPos, N, albedo, specular_color, reflected_dir, view_dir;
 // //float  roughness;
 // //decodeGBuffer( iPosition.xy, wPos, N, albedo, specular_color, roughness, reflected_dir, view_dir );

 // // if roughness = 0 -> I want to use the miplevel 0, the all-detailed image
 // // if roughness = 1 -> I will use the most blurred image, the 8-th mipmap, If image was 256x256 => 1x1
 // float mipIndex = roughness * roughness * 8.0f;
 // float3 env = txEnvironmentMap.SampleLevel(samLinear, reflected_dir, mipIndex).xyz;
 // // Convert the color to linear also.
 // env = pow(abs(env), 2.2f);

 // // The irrandiance, is read using the N direction.
 // // Here we are sampling using the cubemap-miplevel 4, and the already blurred txIrradiance texture
 // // and mixing it in base to the scalar_irradiance_vs_mipmaps which comes from the ImGui.
 // // Remove the interpolation in the final version!!!
 // float3 irradiance_mipmaps = txEnvironmentMap.SampleLevel(samLinear, N, 4).xyz;
 // float3 irradiance_texture = txIrradianceMap.Sample(samLinear, N).xyz;
 // float3 irradiance = irradiance_texture * scalar_irradiance_vs_mipmaps + irradiance_mipmaps * ( 1. - scalar_irradiance_vs_mipmaps );

 // // How much the environment we see
 // float3 env_fresnel = Specular_F_Roughness(specular_color, 1. - roughness * roughness, N, view_dir);
 // //return float4(env_fresnel, 1 );

 // float g_ReflectionIntensity = 1.0;
 // float g_AmbientLightIntensity = 1.0;

 // float ao = txAO.Sample( samLinear, iTex0).x;

 // float4 self_illum = txSelfIllum.Load(uint3(Pos.xy,0));

 // float4 final_color = float4(env_fresnel * env * g_ReflectionIntensity + 
 //                             o_albedo.xyz * irradiance * g_AmbientLightIntensity
 //                             , 0.5f) + self_illum;


 // //final_color =  final_color * global_ambient_adjustment;// * ao;
 // ////return float4(0,0,0,1);
 //final_color = lerp(float4(env, 1), final_color, 1) + float4(self_illum.xyz, 1) * global_ambient_adjustment;
 // //final_color.a = 0.3f;
 // return final_color;
}
