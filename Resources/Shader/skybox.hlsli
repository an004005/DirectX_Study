#ifndef _SKYBOX_HLSLI_
#define _SKYBOX_HLSLI_
#include "params.hlsli"

struct VS_IN
{
	float3 localPos : POSITION;
	float2 uv : TEXCOORD;
};

struct VS_OUT
{
	float4 pos : SV_Position;
	float2 uv : TEXCOORD;
};

VS_OUT VS_Main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;

	// Translation은 하지 않고 Rotation만 적용한다, 항상0,0,0(카메라위치)이므로 world변환 할 필요없음
	float4 viewPos = mul(float4(input.localPos, 0), g_matView);
	float4 clipSpacePos = mul(viewPos, g_matProjection);
	// 무조건 1이 나오게 해야한다.
	// w/w=1이기 때문에 항상 깊이가 1로 유지된다(z값을 깊이로 쓰는데, z값을 1로 줘서 항상 1로 되게함)
	output.pos = clipSpacePos.xyww;
	output.uv = input.uv;

	return output;
}

float4 PS_Main(VS_OUT input) : SV_Target
{
	 float4 color = g_tex_0.Sample(g_sam_0, input.uv);
	 return color;
}

#endif
