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

	void AddGameObject(shared_ptr<GameObject> gameObject);
	void RemoveGameObject(shared_ptr<GameObject> gameObject);

	const vector<shared_ptr<GameObject>>& GetGameObjects() { return _gameObjects;  }

private:
	// 레이어드를 둬서 원하는 오브젝트 찾기 쉽게 할 수 있음
	vector<shared_ptr<GameObject>> _gameObjects;
};

