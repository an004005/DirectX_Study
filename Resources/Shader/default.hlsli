
// https://docs.microsoft.com/en-us/windows/win32/direct3d12/example-root-signatures
// 위 문서 첫번째 그림의 b0(레지스터의 이름)
// constant buffer로 사용하는 레지스터는 b로 시작함
// 그림의 노란부분은 어떤 용도로 쓸지 정함(타입)
cbuffer TEST_B0 : register(b0)
{
	float4 offset0;
};

cbuffer TEST_B1 : register(b1)
{
	float4 offset1;
};

Texture2D tex_0: register(t0);

SamplerState sam_0: register(s0);

struct VS_IN
{
	float3 pos : POSITION;
	float4 color : COLOR;
	float2 uv : TEXCOORD;
};

struct VS_OUT
{
	float4 pos : SV_Position;
	float4 color : COLOR;
	float2 uv : TEXCOORD;
};

VS_OUT VS_Main(VS_IN input) // vertex shader 단계에서 할 일
{
	VS_OUT output = (VS_OUT)0;

	output.pos = float4(input.pos, 1.f); // 마지막 값만 1로 채워서 float4로 만듬
	output.color = input.color; // 컬러 안바꿈
	output.uv = input.uv;
	
	return output;
}

float4 PS_Main(VS_OUT input) : SV_Target // pixel shader 단계에서 할 일
{
	float4 color = tex_0.Sample(sam_0, input.uv);
	return color;
}