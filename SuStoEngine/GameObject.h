#ifndef __GameObject_H__
#define __GameObject_H__

#include <iostream>
#include <vector>
#include "Component.h"

#include "ComponentTransform.h"
#include "ResourcePrefab.h"

class ModuleGameObject;
class UICanvas;
class GameObjectAbstractor;
class Event;

class GameObject
{
	friend ModuleGameObject;
	friend GameObjectAbstractor;

public:
	GameObject();
	GameObject(std::string unique_id);
	virtual ~GameObject();

	void Start();
	void PreUpdate();
	void UpdateComponents();
	void Update();
	void PostUpdate();
	void Draw();
	void DestroyComponents();
	void CleanUp();

	void Enable();
	void Disable();
	const bool GetEnabled() const;
	void SetEnabled(const bool& set);

	Component* AddComponent(ComponentType type, std::string unique_id = "");
	bool RemoveComponent(ComponentType type);
	bool RemoveComponent(Component* comp);
	bool HasComponent(ComponentType type);
	uint ComponentTypeCount(ComponentType type);
	std::vector<Component*> GetComponents();
	Component* GetComponent(ComponentType type);

	const char* GetName() const;
	void SetName(const std::string& set);

	std::string GetUniqueId();
	const bool GetSelected() const;
	const bool GetStatic() const;

	GameObject* GetParent() const;
	std::vector<GameObject*> GetChilds() const;
	const uint GetChildsCount() const;
	void SetParent(GameObject* parent);
	void EraseParent();
	bool HasChild(GameObject* child);

	void RecursiveCalcGlobalTransform();
	void RecursiveCalcBBox();
	void RecursiveTestRay(const LineSegment& segment, GameObject*& closest, float& dist);
	void TestRay(const LineSegment& segment, bool& hit, float& dist);
	float4x4 GetParentTransform();

	void SetPrefabInstance(ResourcePrefab* prefab_instance);
	ResourcePrefab* GetPrefabInstance() const;
	void UpdatePrefabInstance();

	AABB GetBbox() const;

	void OnLoadAbstraction(DataAbstraction& abs);
	void OnSaveAbstraction(DataAbstraction& abs);

	void SetDraw(bool set);

	void SetDebugDraw(bool set);

private:
	void DrawBBox();

public:
	ComponentTransform* transform = nullptr;
 
private:
	std::string		unique_id;
	std::string		name;
	bool			enabled = false;
	bool			selected = false;
	bool			is_static = false;
	bool			draw = false;

	std::vector<Component*> components;
	std::list<Component*> components_to_destroy;

	AABB	        local_bbox;

	GameObject*		parent = nullptr;
	std::vector<GameObject*> childs;

	ResourcePrefab* prefab_instance = nullptr;

	bool		    is_ui = false;

	bool		    debug_draw = false;
};

#endif