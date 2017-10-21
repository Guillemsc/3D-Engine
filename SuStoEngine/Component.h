#ifndef __Component_H__
#define __Component_H__

#include "Globals.h"
#include <vector>

class GameObject;

enum ComponentType
{
	TRANSFORM,
	MESH,
	MATERIAL,
	CAMERA,
	PRIMITIVE,
};

class Component
{
public:
	Component(ComponentType type, GameObject* owner);
	virtual ~Component();

	virtual void Start() {};
	virtual void Update() {};
	virtual void CleanUp() {};

	const GameObject* GetOwner() const;
	const ComponentType GetType() const;
	const char* GetName() const;

	void Enable();
	void Disable();
	const bool GetEnabled() const;
	void SetEnabled(const bool& set);
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