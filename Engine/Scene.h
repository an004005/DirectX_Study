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
	// ���̾�带 �ּ� ���ϴ� ������Ʈ ã�� ���� �� �� ����
	vector<shared_ptr<GameObject>> _gameObjects;
};
