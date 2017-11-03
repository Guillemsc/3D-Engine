#ifndef __GameObject_H__
#define __GameObject_H__

#include <iostream>
#include <vector>
#include "Component.h"

#include "ComponentTransform.h"

class GameObject
{
public:
	GameObject(double id);
	virtual ~GameObject();

	void Start();
	void Update();
	void Draw();

	void Enable();
	void Disable();
	const bool GetEnabled() const;
	void SetEnabled(const bool& set);
	void CleanUp();

	void AddComponent(ComponentType type);
	void RemoveComponent(ComponentType type);
	bool ContainsComponent(ComponentType type);
	std::vector<Component*> GetComponents();

	Component* GetComponent(ComponentType type);

	const char* GetName() const;
	void SetName(const std::string& set);
	void SetSelected(const bool& set);

	const int GetId() const;
	const bool GetSelected() const;

	const GameObject* GetParent() const;
	GameObject* GetParent();
	const std::vector<GameObject*> GetChilds() const;
	const uint GetChildsCount() const;
	const void SetParentToNull();
	void EraseChild(GameObject* child, bool send_child_to_root = true);
	void AddChild(GameObject* child);

	void RecursiveCalcGlobalTransform();
	void RecursiveCalcBBox();

private:
	void DrawBBox();

public:
	ComponentTransform* transform = nullptr;
	
private:
	double		id = 0;
	std::string name;
	bool		enabled = false;
	bool		selected = false;

	std::vector<Component*> components;

	GameObject* parent = nullptr;
	std::vector<GameObject*> childs;

	AABB	     local_bbox;
};

#endif