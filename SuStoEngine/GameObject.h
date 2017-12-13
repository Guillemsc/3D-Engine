#ifndef __GameObject_H__
#define __GameObject_H__

#include <iostream>
#include <vector>
#include "Component.h"

#include "ComponentTransform.h"

class ModuleGameObject;
class UICanvas;

class GameObject
{
	friend ModuleGameObject;

public:
	GameObject(std::string unique_id, ModuleGameObject* go_module);
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
	const void SetParentToNull();
	void EraseChild(GameObject* child, bool send_child_to_root = true);
	void AddChild(GameObject* child);

	void RecursiveCalcGlobalTransform();
	void RecursiveCalcBBox();
	void RecursiveTestRay(const LineSegment& segment, GameObject*& closest, float& dist);
	void TestRay(const LineSegment& segment, bool& hit, float& dist);
	float4x4 GetParentTransform();

	AABB GetBbox() const;

	UICanvas* RecursiveFindCanvasOnParent();

	void OnChangeParent();

	void OnSaveSerialize(JSON_Doc config);
	void OnLoadSerialize(JSON_Doc config);

	void SetDraw(bool set);

	void SetDebugDraw(bool set);
 
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

	ModuleGameObject* go_module = nullptr;

	bool debug_draw = false;
};

#endif