#ifndef __GameObject_H__
#define __GameObject_H__

#include <iostream>
#include <vector>
#include "Component.h"

#include "ComponentTransform.h"

class ModuleGameObject;
class UICanvas;
class GameObjectAbstractor;
class Event;

class GameObject
{
	friend ModuleGameObject;
	friend GameObjectAbstractor;

public:
	GameObject(std::string unique_id, ModuleGameObject* go_module);
	virtual ~GameObject();

	void Start();
	void UpdateComponents();
	void Draw();
	void UpdateLogic();
	void CleanUp();

	void Enable();
	void Disable();
	const bool GetEnabled() const;
	void SetEnabled(const bool& set);

	Component* AddComponent(ComponentType type, std::string unique_id = "");
	void RemoveComponent(ComponentType type);
	bool ContainsComponent(ComponentType type);
	std::vector<Component*> GetComponents();
	Component* GetComponent(ComponentType type);

	const char* GetName() const;
	void SetName(const std::string& set);

	std::string GetUniqueId();
	const bool GetSelected() const;
	const bool GetStatic() const;

	const GameObject* GetParent() const;
	GameObject* GetParent();
	const std::vector<GameObject*> GetChilds() const;
	const uint GetChildsCount() const;
	void EraseParent(bool send_to_root = true);
	void EraseChild(GameObject* child, bool send_child_to_root = true);
	void AddChild(GameObject* child);
	bool HasChild(GameObject* child);

	void RecursiveCalcGlobalTransform();
	void RecursiveCalcBBox();
	void RecursiveTestRay(const LineSegment& segment, GameObject*& closest, float& dist);
	void TestRay(const LineSegment& segment, bool& hit, float& dist);
	float4x4 GetParentTransform();

	AABB GetBbox() const;

	void SetIsUI(const bool & set);
	bool IsUI() const;

	void OnChangeParent();

	void OnLoadAbstraction(DataAbstraction& abs);
	void OnSaveAbstraction(DataAbstraction& abs);

	void SetDraw(bool set);

	void SetDebugDraw(bool set);

	void OnChangeParent(Event ev);
	void OnDestroy(Event ev);

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
	bool		draw = false;

	std::vector<Component*> components;

	AABB	    local_bbox;

	GameObject* parent = nullptr;
	std::vector<GameObject*> childs;

	bool		is_ui = false;

	ModuleGameObject* go_module = nullptr;

	bool debug_draw = false;
};

#endif