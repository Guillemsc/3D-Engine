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
	bool ContainsComponent(ComponentType type);
	std::vector<Component*> GetComponents();

	Component* FindComponentByType(ComponentType type);

	const char* GetName() const;
	void SetName(std::string set);
	void SetSelected(bool set);

	int GetId();
	bool GetSelected();

public:
	Component* transform = nullptr;
	
private:
	std::string name;
	bool		enabled = false;
	int			id = 0;
	bool		selected = false;

	std::vector<Component*> components;

	GameObject* parent = nullptr;
};

#endif