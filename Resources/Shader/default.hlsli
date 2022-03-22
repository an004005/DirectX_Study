

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

VS_OUT VS_Main(VS_IN input) // vertex shader �ܰ迡�� �� ��
{
	VS_OUT output = (VS_OUT)0;

	output.pos = float4(input.pos, 1.f); // ������ ���� 1�� ä���� float4�� ����
	output.color = input.color; // �÷� �ȹٲ�
	
	return output;
}

float4 PS_Main(VS_OUT input) : SV_Target // pixel shader �ܰ迡�� �� ��
{
	return input.color;
}