#pragma once

class GameObject;

class Scene
{
public:
	void Awake();
	void Start();
	void Update();
	void LateUpdate();
	void FinalUpdate();

	void Render();
	void RenderLights();
	void RenderFinal();

	void AddGameObject(shared_ptr<GameObject> gameObject);
	void RemoveGameObject(shared_ptr<GameObject> gameObject);

	const vector<shared_ptr<GameObject>>& GetGameObjects() { return _gameObjects;  }

private:
	void PushLightData();

private:
	// 레이어드를 둬서 원하는 오브젝트 찾기 쉽게 할 수 있음
	vector<shared_ptr<GameObject>>		_gameObjects;
	// 빠르게 찾으려고 캐싱
	vector<shared_ptr<class Camera>>	_cameras;
	vector<shared_ptr<class Light>>		_lights;
};

