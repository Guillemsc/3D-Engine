#ifndef __Component_H__
#define __Component_H__

#include "Globals.h"
#include <vector>
#include <string>
#include "GeometryMath.h"

class GameObject;
class JSON_Doc;

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

	virtual void OnGetBoundingBox(AABB &bbox) {};
	virtual void InspectorDraw(std::vector<Component*> components) {};

	virtual void LoadScene(JSON_Doc* config) {};
	virtual void SaveScene(JSON_Doc* config, std::string root) {};

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