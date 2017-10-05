#ifndef __Component_H__
#define __Component_H__

class GameObject;

enum ComponentType
{
	TRANSFORM
};

class Component
{
public:
	Component(ComponentType type, GameObject* owner);
	virtual ~Component();

	GameObject* GetOwner();
	ComponentType GetType();

	void Enable();
	void Disable();
	bool GetEnabled();
	void SetEnabled(bool set);
	void Destroy();

private:
	virtual void OnCreate() {};
	virtual void OnDestroy() {};
	virtual void OnEnable() {};
	virtual void OnDisable() {};

private:
	bool		  enabled = false;
	GameObject*   owner = nullptr;
	ComponentType type;
};

#endif