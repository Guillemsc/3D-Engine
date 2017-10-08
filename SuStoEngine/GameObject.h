#ifndef __GameObject_H__
#define __GameObject_H__

#include <iostream>
#include <vector>
#include "Component.h"

class GameObject
{
public:
	GameObject(int id);
	virtual ~GameObject();

	void Start();
	void Update();

	void Enable();
	void Disable();
	bool GetEnabled();
	void SetEnabled(bool set);
	void CleanUp();

	void AddComponent(ComponentType type);
	void RemoveComponent(ComponentType type);
	std::vector<Component*> GetComponents();

	Component* FindComponentByType(ComponentType type);

	const char* GetName() const;
	void SetName(const char* new_name);
	void SetSelected(bool set);

	int GetId();
	bool GetSelected();

public:
	
private:
	char		name[25] = "";
	bool		enabled = false;
	int			id = 0;
	bool		selected = false;

	std::vector<Component*> components;

	GameObject* parent = nullptr;
};

#endif