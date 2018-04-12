#include "common.fx"

//--------------------------------------------------------------------------------------
// This shader is expected to be used only with the mesh unitQuadXY.mesh
// Where the iPos goes from 0,0..1,1
void VS(
    in float4 iPos : POSITION
  , in float2 iColor : COLOR0
  , out float4 oPos : SV_POSITION
  , out float2 oTex0 : TEXCOORD0
)
{
  // Passthrough of coords and UV's
  oPos = float4(iPos.x * 2 - 1., 1 - iPos.y * 2, 0, 1);
  oTex0 = iPos.xy;
  /*
  // Save in the xy the positive offset 
  // Save in the zw the negative offset 
  oTex1.xy = oTex0 + blur_step * blur_d.x;
  oTex1.zw = oTex0 - blur_step * blur_d.x;
  oTex2.xy = oTex0 + blur_step * blur_d.y;
  oTex2.zw = oTex0 - blur_step * blur_d.y;
  oTex3.xy = oTex0 + blur_step * blur_d.z;
  oTex3.zw = oTex0 - blur_step * blur_d.z;
  */
}

//--------------------------------------------------------------------------------------
float4 PS(
    in float4 iPosition : SV_Position
  , in float2 iTex0 : TEXCOORD0
) : SV_Target
{

  // 7 tap blur controlled by the vs
  float4 cfinal  = txAlbedo.Sample(samClampLinear, iTex0.xy) * 1.2;

/*
  float4 cfinal =
      (c0       ) * blur_w.x

    + (cp1 + cn1) * blur_w.y
    + (cp2 + cn2) * blur_w.z
    + (cp3 + cn3) * blur_w.w
    ;
*/
  return cfinal;
}
