#pragma once

class Timer
{
public:
	void Init();
	void Update();

	uint32 GetFps() { return _fps; }
	float GetDeltaTime() { return _deltaTime; }

private:
	uint64	_frequency = 0;
	uint64	_prevCount = 0;

	// deltaTime: 이전프레임에서 현재 프레임까지 경과된 시간
	float	_deltaTime = 0.f;

private:
	// 프레임 계산 용도
	uint32	_frameCount = 0;
	float	_frameTime = 0.f;

	// frame per second(매초마다 몇프레임이 실행되었는지)
	uint32	_fps = 0;
};

