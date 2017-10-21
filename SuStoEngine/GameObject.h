#ifndef __GameObject_H__
#define __GameObject_H__

#include <iostream>
#include <vector>
#include "Component.h"

#include "ComponentTransform.h"

class GameObject
{
public:
	GameObject(int id);
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
	const std::vector<Component*> GetComponents() const;

	Component* FindComponentByType(ComponentType type);

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
	void RecursiveCalcBBoxTransform();

public:
	ComponentTransform* transform = nullptr;
	
private:
	std::string name;
	bool		enabled = false;
	int			id = 0;
	bool		selected = false;

	std::vector<Component*> components;

	GameObject* parent = nullptr;
	std::vector<GameObject*> childs;
};

#endif