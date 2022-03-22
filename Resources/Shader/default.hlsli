

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
	output.color = input.color; // 컬러 안바꿈
	
	return output;
}

float4 PS_Main(VS_OUT input) : SV_Target // pixel shader 단계에서 할 일
{
	return input.color;
}