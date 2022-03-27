#ifndef _DEFAULT_HLSLI_
#define _DEFAULT_HLSLI_
#include "params.hlsli"
#include "utils.hlsli"

struct VS_IN
{
	float3 pos : POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
};

struct VS_OUT
{
	float4 pos : SV_Position; // SV_Position은 예약어라서 바꿀 수 없다.
	float2 uv : TEXCOORD;
	float3 viewPos : POSITION;
	float3 viewNormal : NORMAL;
};

VS_OUT VS_Main(VS_IN input) // vertex shader 단계에서 할 일(정점단위 연산)
{
	VS_OUT output = (VS_OUT)0;

	output.pos = mul(float4(input.pos, 1.f), g_matWVP); // 마지막 값만 1로 채워서 좌표의 개념으로 사용(0이면 방향성만사용)
	output.uv = input.uv;

	output.viewPos = mul(float4(input.pos, 1.f), g_matWV).xyz; // 빛 계산으로 view space에서하면, camera좌표를 안써도 되서 좋다.
	output.viewNormal = normalize(mul(float4(input.normal, 0.f), g_matWV).xyz);
	
	return output;
}

float4 PS_Main(VS_OUT input) : SV_Target // pixel shader 단계에서 할 일
{
	// float4 color = g_tex_0.Sample(g_sam_0, input.uv);
	float4 color = float4(1.f, 1.f, 1.f, 1.f);

	LightColor totalColor = (LightColor)0.f;

	for (int i = 0; i < g_lightCount; ++i)
	{
		LightColor color = CalculateLightColor(i, input.viewNormal, input.viewPos);
		totalColor.diffuse += color.diffuse;
		totalColor.ambient += color.ambient;
		totalColor.specular += color.specular;
	}

	color.xyz = (totalColor.diffuse.xyz * color.xyz)
		+ totalColor.ambient.xyz * color.xyz
		+ totalColor.specular.xyz;

	return color;
}

#endif
