#ifndef __GameObject_H__
#define __GameObject_H__

#include <iostream>
#include <vector>
#include "Component.h"

#include "ComponentTransform.h"

class GameObject
{
public:
	GameObject(std::string unique_id);
	virtual ~GameObject();

	void Start();
	void UpdateComponents();
	void Draw();
	void UpdateLogic();

	void Enable();
	void Disable();
	const bool GetEnabled() const;
	void SetEnabled(const bool& set);
	void CleanUp();

	void AddComponent(ComponentType type, std::string unique_id = "");
	void RemoveComponent(ComponentType type);
	bool ContainsComponent(ComponentType type);
	std::vector<Component*> GetComponents();

	Component* GetComponent(ComponentType type);

	const char* GetName() const;
	void SetName(const std::string& set);
	void SetSelected(const bool& set);

	std::string GetUniqueId();
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
	void RecursiveTestRay(const LineSegment& segment, GameObject*& closest, float& dist);

	bool GetStatic();
	void SetStatic(bool set);
	AABB GetBbox() const;

	void OnLoadScene(JSON_Doc* config);
	void OnSaveScene(JSON_Doc* config);
 
private:
	void DrawBBox();

public:
	ComponentTransform* transform = nullptr;
 
private:
	std::string unique_id;
	std::string name;
	bool		enabled = false;
	bool		selected = false;
	bool		is_static = false;

	std::vector<Component*> components;

	AABB	    local_bbox;

	GameObject* parent = nullptr;
	std::vector<GameObject*> childs;
};

#endif