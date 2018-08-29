#include "common.fx"

//--------------------------------------------------------------------------------------
// Alpha generation pass. Vertex
//--------------------------------------------------------------------------------------
void VS_FogRight(
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
    oTex0 = float2((iTex0.x - global_world_time * 0.01), iTex0.y );
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
float4 PS_FogRight(
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
    float4 final_color = txAlbedo.Sample(samLinear, iTex0);
    float4 alpha_color = txAlpha.Sample(samLinear, iTex0); // * txAlpha.Sample(samLinear, iTex0) + float4(0, 0, 0, 0) * (1. - txAlpha.Sample(samLinear, iTex0)); /** (1. - txTransparency.Sample(samLinear, iTex0))*/; //obj_color.a;

    final_color.a = alpha_color.r;
    if (final_color.a < 0.3) 
        discard;
		
	if (iTex1.x < 0.15)
		final_color.w = final_color.w * (iTex1.x /0.15);
	if (iTex1.x > 0.85)
		final_color.w = final_color.w * ((1-iTex1.x) /0.15);
		
		
    return final_color;
 
}
