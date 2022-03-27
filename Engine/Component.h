#pragma once

enum class COMPONENT_TYPE : uint8
{
	TRANSFORM,
	MESH_RENDERER,
	CAMERA,
	// ...
	MONO_BEHAVIOUR,
	END,
};

enum
{
	// mono behaviour빼고 각 1개씩 가능
	FIXED_COMPONENT_COUNT = static_cast<uint8>(COMPONENT_TYPE::END) - 1 // MONO_BEHAVIOUR 제외
};

class GameObject;
class Transform;

class Component
{
public:
	Component(COMPONENT_TYPE type);
	virtual ~Component();

public:
	virtual void Awake() { }
	virtual void Start() { }
	virtual void Update() { }
	virtual void LateUpdate() { }
	virtual void FinalUpdate() { } // 엔진에서만 쓰는 진짜 마지막 단계

public:
	COMPONENT_TYPE GetType() { return _type; }
	bool IsValid() { return _gameObject.expired() == false; }

	shared_ptr<GameObject> GetGameObject();
	shared_ptr<Transform> GetTransform();

private:
	friend class GameObject; // gameObject만 SetGameObject실행 가능
	void SetGameObject(shared_ptr<GameObject> gameObject) { _gameObject = gameObject; }

protected:
	COMPONENT_TYPE _type;
	// gameObject도 Component를 알 수 있어야함. shared_ptr로 하면 순환구조가 되서 안됨
	weak_ptr<GameObject> _gameObject;
};

