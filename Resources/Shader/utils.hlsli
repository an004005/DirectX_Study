#ifndef _UTILS_HLSLI_
#define _UTILS_HLSLI_

// 몇번째 광원인지, view space기준 normal과 view space 기준 position가져옴
LightColor CalculateLightColor(int lightIndex, float3 viewNormal, float3 viewPos)
{
	LightColor color = (LightColor)0.f;

	float3 viewLightDir = (float3)0.f;

	float diffuseRatio = 0.f;
	float specularRatio = 0.f;
	float distanceRatio = 1.f;

	if (g_light[lightIndex].lightType == 0)
	{
		// Directional Light

		// light의 방향을 view space로 이동
		viewLightDir = normalize(mul(float4(g_light[lightIndex].direction.xyz, 0.f), g_matView).xyz);
		// normal과 direction을 내적(음수면 0으로 고정하기위해 saturator사용)
		// 빛이 반대방향으로 오면 빛을 못 받으므로 0으로(dot이 0이 나옴)
		diffuseRatio = saturate(dot(-viewLightDir, viewNormal));
	}
	else if (g_light[lightIndex].lightType == 1)
	{
		// Point Light

		// 광원 위치
		float3 viewLightPos = mul(float4(g_light[lightIndex].position.xyz, 1.f), g_matView).xyz;
		// 광원에서 타겟으로 가는 벡터
		viewLightDir = normalize(viewPos - viewLightPos);
		diffuseRatio = saturate(dot(-viewLightDir, viewNormal));

		// 거리에 따른 감소 적용
		float dist = distance(viewPos, viewLightPos);
		if (g_light[lightIndex].range == 0.f)
			distanceRatio = 0.f;
		else
			distanceRatio = saturate(1.f - pow(dist / g_light[lightIndex].range, 2));
	}
	else
	{
		// Spot Light
		float3 viewLightPos = mul(float4(g_light[lightIndex].position.xyz, 1.f), g_matView).xyz;
		viewLightDir = normalize(viewPos - viewLightPos);
		diffuseRatio = saturate(dot(-viewLightDir, viewNormal));

		if (g_light[lightIndex].range == 0.f)
			distanceRatio = 0.f;
		else
		{
			// 시야각
			float halfAngle = g_light[lightIndex].angle / 2;

			float3 viewLightVec = viewPos - viewLightPos;
			// 광원이 나가는 방향(정방향), 방향벡터이기 때문에 마지막 값 w 0셋팅
			float3 viewCenterLightDir = normalize(mul(float4(g_light[lightIndex].direction.xyz, 0.f), g_matView).xyz);

			// center distance, viewLightVec을 viewCenterLightDir로 내린 길이
			float centerDist = dot(viewLightVec, viewCenterLightDir);
			distanceRatio = saturate(1.f - centerDist / g_light[lightIndex].range);

			// 타겟과 center의 각도
			float lightAngle = acos(dot(normalize(viewLightVec), viewCenterLightDir));

			if (centerDist < 0.f || centerDist > g_light[lightIndex].range) // 최대 거리를 벗어났는지
				distanceRatio = 0.f;
			else if (lightAngle > halfAngle) // 최대 시야각을 벗어났는지
				distanceRatio = 0.f;
			else // 거리에 따라 적절히 세기를 조절
				distanceRatio = saturate(1.f - pow(centerDist / g_light[lightIndex].range, 2));
		}
	}

	// specular계산
	// (saturate(dot(-viewLightDir, viewNormal))로 cos 구하고 *2하고 viewNormal벡터에 곱하고 나서 viewLightDir에 더해서 반사광을 구한다.
	float3 reflectionDir = normalize(viewLightDir + 2 * (saturate(dot(-viewLightDir, viewNormal)) * viewNormal));
	float3 eyeDir = normalize(viewPos); //viewPos는 view space 이기 때문에 자체로 카메라에서 타겟을 항하는 벡터이다.
	specularRatio = saturate(dot(-eyeDir, reflectionDir)); // 카메라쪽으로 가는 벡터를 위해 -취해
	specularRatio = pow(specularRatio, 2); // 0~1  이라 값 작아짐, 1에 가까울 수록 반짝임

	color.diffuse = g_light[lightIndex].color.diffuse * diffuseRatio * distanceRatio;
	color.ambient = g_light[lightIndex].color.ambient * distanceRatio;
	color.specular = g_light[lightIndex].color.specular * specularRatio * distanceRatio;

	return color;
}
#endif