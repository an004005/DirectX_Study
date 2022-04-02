#ifndef _PARAMS_FX_
#define _PARAMS_FX_

// https://docs.microsoft.com/en-us/windows/win32/direct3d12/example-root-signatures
// 위 문서 첫번째 그림의 b0(레지스터의 이름)
// constant buffer로 사용하는 레지스터는 b로 시작함
// 그림의 노란부분은 어떤 용도로 쓸지 정함(타입)
struct LightColor
{
	float4      diffuse;
	float4      ambient;
	float4      specular;
};

struct LightInfo
{
	LightColor  color;
	float4	    position;
	float4	    direction;
	int		    lightType;
	float	    range;
	float	    angle;
	int  	    padding;

	// https://docs.microsoft.com/en-us/windows/win32/direct3dhlsl/dx-graphics-hlsl-packing-rules
	// hlsl에서는 padding 쓰이는 이유: 16byte단위로 정렬이 되기 때문.
	// c++에서도 똑같이 해서 주는게 좋음
	// Vec4를 쓰고, 맨뒤 값을 0으로 두는 이유도 이것임.
};

cbuffer GLOBAL_PARAMS : register(b0)
{
	int         g_lightCount;
	float3      g_lightPadding;
	LightInfo   g_light[50];
}

cbuffer TRANSFORM_PARAMS : register(b1)
{
	// row_major : matrix규직을 dx기준으로 사용하기 위함(dx는 행렬 접근시 가로방향으로
	// shader에서는 세로방향으로 접근하는데, shader도 가로방향으로 접근하라는 힌트를 준것임
	row_major matrix g_matWorld;
	row_major matrix g_matView;
	row_major matrix g_matProjection;
	row_major matrix g_matWV;
	row_major matrix g_matWVP;
	
};

cbuffer MATERIAL_PARAMS : register(b2)
{
	int g_int_0;
	int g_int_1;
	int g_int_2;
	int g_int_3;
	int g_int_4;
	float g_float_0;
	float g_float_1;
	float g_float_2;
	float g_float_3;
	float g_float_4;
	int g_tex_on_0;
	int g_tex_on_1;
	int g_tex_on_2;
	int g_tex_on_3;
	int g_tex_on_4;
};

Texture2D g_tex_0: register(t0);
Texture2D g_tex_1: register(t1);
Texture2D g_tex_2: register(t2);
Texture2D g_tex_3: register(t3);
Texture2D g_tex_4: register(t4);

SamplerState g_sam_0: register(s0);

#endif