
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

struct VS_IN
{
	float3 pos : POSITION;
	float4 color : COLOR;
};

struct VS_OUT
{
	float4 pos : SV_Position;
	float4 color : COLOR;
};

VS_OUT VS_Main(VS_IN input) // vertex shader 단계에서 할 일
{
	VS_OUT output = (VS_OUT)0;

	output.pos = float4(input.pos, 1.f); // 마지막 값만 1로 채워서 float4로 만듬
	output.pos += offset0;
	output.color = input.color; // 컬러 안바꿈
	output.color += offset1;
	
	return output;
}

float4 PS_Main(VS_OUT input) : SV_Target // pixel shader 단계에서 할 일
{
	return input.color;
}