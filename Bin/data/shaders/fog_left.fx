#include "common.fx"

//--------------------------------------------------------------------------------------
// Alpha generation pass. Vertex
//--------------------------------------------------------------------------------------
void VS_FogLeft(
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
    oTex0 = float2((iTex0.x + global_world_time * 0.01), iTex0.y );
    oTex1 = iTex1;
    oWorldPos = world_pos.xyz;

}

//--------------------------------------------------------------------------------------
// Alpha generation pass. Pixel shader
//--------------------------------------------------------------------------------------
float4 PS_FogLeft(
  float4 Pos : SV_POSITION
, float3 iNormal : NORMAL0
, float4 iTangent : NORMAL1
, float2 iTex0 : TEXCOORD0
, float2 iTex1 : TEXCOORD1
, float3 iWorldPos : TEXCOORD2
) : SV_Target0
{
    float4 final_color = txAlbedo.Sample(samLinear, iTex0);
    float4 alpha_color = txAlpha.Sample(samLinear, iTex0);// * txAlpha.Sample(samLinear, iTex0) + float4(0, 0, 0, 0) * (1. - txAlpha.Sample(samLinear, iTex0)); /** (1. - txTransparency.Sample(samLinear, iTex0))*/; //obj_color.a;
	
    final_color.a = alpha_color.r;
	
    if (iTex1.x < global_fog_percentage_horizontal)
        final_color.w = final_color.w * (iTex1.x / global_fog_percentage_horizontal);
    if (iTex1.x > 1 - global_fog_percentage_horizontal)
        final_color.w = final_color.w * ((1 - iTex1.x) / global_fog_percentage_horizontal);
    if (iTex1.y < global_fog_percentage_vertical)
        final_color.w = final_color.w * (iTex1.y / global_fog_percentage_vertical);
    if (iTex1.y > 1 - global_fog_percentage_vertical)
        final_color.w = final_color.w * ((1 - iTex1.y) / global_fog_percentage_vertical);
		
    return final_color;

}
