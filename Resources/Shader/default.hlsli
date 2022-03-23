
// https://docs.microsoft.com/en-us/windows/win32/direct3d12/example-root-signatures
// �� ���� ù��° �׸��� b0(���������� �̸�)
// constant buffer�� ����ϴ� �������ʹ� b�� ������
// �׸��� ����κ��� � �뵵�� ���� ����(Ÿ��)
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

VS_OUT VS_Main(VS_IN input) // vertex shader �ܰ迡�� �� ��
{
	VS_OUT output = (VS_OUT)0;

	output.pos = float4(input.pos, 1.f); // ������ ���� 1�� ä���� float4�� ����
	output.color = input.color; // �÷� �ȹٲ�
	output.uv = input.uv;
	
	return output;
}

float4 PS_Main(VS_OUT input) : SV_Target // pixel shader �ܰ迡�� �� ��
{
	float4 color = tex_0.Sample(sam_0, input.uv);
	return color;
}