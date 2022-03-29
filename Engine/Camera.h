#pragma once
#include "Component.h"
#include "Frustum.h"

enum class PROJECTION_TYPE
{
	PERSPECTIVE, // 원근 투영
	ORTHOGRAPHIC, // 직교 투영, 원근법 안씀
};

class Camera : public Component
{
public:
	Camera();
	virtual ~Camera();

	virtual void FinalUpdate() override;
	void Render();

private:
	PROJECTION_TYPE _type = PROJECTION_TYPE::PERSPECTIVE;

	float _near = 1.f; // n
	float _far = 1000.f; // f
	float _fov = XM_PI / 4.f; // 찍는 영역
	float _scale = 1.f;

	Matrix _matView = {};
	Matrix _matProjection = {};

	Frustum _frustum;

public:
	// TEMP
	static Matrix S_MatView;
	static Matrix S_MatProjection;
};

