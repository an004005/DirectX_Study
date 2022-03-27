
// https://docs.microsoft.com/en-us/windows/win32/direct3d12/example-root-signatures
// 위 문서 첫번째 그림의 b0(레지스터의 이름)
// constant buffer로 사용하는 레지스터는 b로 시작함
// 그림의 노란부분은 어떤 용도로 쓸지 정함(타입)
cbuffer TRANSFORM_PARAMS : register(b0)
{
	// row_major : matrix규직을 dx기준으로 사용하기 위함(dx는 행렬 접근시 가로방향으로
	// shader에서는 세로방향으로 접근하는데, shader도 가로방향으로 접근하라는 힌트를 준것임
	row_major matrix matWVP;
	
};

cbuffer MATERIAL_PARAMS : register(b1)
{
	int int_0;
	int int_1;
	int int_2;
	int int_3;
	int int_4;
	float float_0;
	float float_1;
	float float_2;
	float float_3;
	float float_4;
};

Texture2D tex_0: register(t0);
Texture2D tex_1: register(t1);
Texture2D tex_2: register(t2);
Texture2D tex_3: register(t3);
Texture2D tex_4: register(t4);

SamplerState sam_0: register(s0);

struct VS_IN
{
	float3 pos : POSITION;
	float2 uv : TEXCOORD;
};

struct VS_OUT
{
	float4 pos : SV_Position;
	float2 uv : TEXCOORD;
};

VS_OUT VS_Main(VS_IN input) // vertex shader 단계에서 할 일
{
	VS_OUT output = (VS_OUT)0;

	output.pos = mul(float4(input.pos, 1.f), matWVP); // 마지막 값만 1로 채워서 좌표의 개념으로 사용(0이면 방향성만사용)
	output.uv = input.uv;
	
	return output;
}

float4 PS_Main(VS_OUT input) : SV_Target // pixel shader 단계에서 할 일
{
	float4 color = tex_0.Sample(sam_0, input.uv);
	return color;
}