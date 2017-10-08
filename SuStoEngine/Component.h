#ifndef __Component_H__
#define __Component_H__

#include "Globals.h"
class GameObject;

enum ComponentType
{
	TRANSFORM,
	PRIMITIVE
};

class Component
{
public:
	Component(ComponentType type, GameObject* owner);
	virtual ~Component();

	virtual void Update() {};

	GameObject* GetOwner();
	ComponentType GetType();
	const char* GetName();

	void Enable();
	void Disable();
	bool GetEnabled();
	void SetEnabled(bool set);
	void SetName(const char* new_name);
	void Destroy();

	virtual void InspectorDraw() {};

private:
	virtual void OnCreate() {};
	virtual void OnDestroy() {};
	virtual void OnEnable() {};
	virtual void OnDisable() {};

private:
	bool		  enabled = false;
	GameObject*   owner = nullptr;
	ComponentType type;
	const char*   name = nullptr;
};

#endif