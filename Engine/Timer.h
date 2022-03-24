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

	// deltaTime: ���������ӿ��� ���� �����ӱ��� ����� �ð�
	float	_deltaTime = 0.f;

private:
	// ������ ��� �뵵
	uint32	_frameCount = 0;
	float	_frameTime = 0.f;

	// frame per second(���ʸ��� ���������� ����Ǿ�����)
	uint32	_fps = 0;
};
