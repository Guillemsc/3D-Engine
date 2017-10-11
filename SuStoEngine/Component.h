#ifndef __Component_H__
#define __Component_H__

#include "Globals.h"
#include <vector>

class GameObject;

enum ComponentType
{
	TRANSFORM,
	PRIMITIVE,
	MESH,
};

class Component
{
public:
	Component(ComponentType type, GameObject* owner);
	virtual ~Component();

	virtual void Start() {};
	virtual void Update() {};
	virtual void CleanUp() {};

	GameObject* GetOwner();
	ComponentType GetType();
	const char* GetName();

	void Enable();
	void Disable();
	bool GetEnabled();
	void SetEnabled(bool set);
	void SetName(const char* new_name);

	virtual void InspectorDraw(std::vector<Component*> components) {};

private:
	virtual void OnEnable() {};
	virtual void OnDisable() {};

private:
	bool		  enabled = false;
	GameObject*   owner = nullptr;
	ComponentType type;
	const char*   name = nullptr;
};

#endif