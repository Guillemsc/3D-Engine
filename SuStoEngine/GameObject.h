#ifndef __GameObject_H__
#define __GameObject_H__

#include <iostream>
#include <list>
#include "Component.h"

class GameObject
{
public:
	GameObject();
	virtual ~GameObject();

	void Update();

	void Enable();
	void Disable();
	bool GetEnabled();
	void SetEnabled(bool set);
	void CleanUp();

	void AddComponent(ComponentType type);
	void RemoveComponent(ComponentType type);
	std::list<Component*> GetComponents();

	Component* FindComponentByType(ComponentType type);

	void SetId(int _id);

	void HierarchyView();

public:
	
private:
	const char* name = nullptr;
	bool		enabled = false;
	int			id = 0;
	std::list<Component*> components;
	char* name;
};

#endif