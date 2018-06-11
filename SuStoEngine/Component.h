#ifndef __Component_H__
#define __Component_H__

#include "Globals.h"
#include <vector>
#include <string>
#include "GeometryMath.h"
#include "JSONLoader.h"
#include "GameObjectAbstractor.h"

class GameObject;

enum ComponentType
{
	TRANSFORM,
	MESH,
	MATERIAL,
	CAMERA,
};

class Component
{
public:
	Component(ComponentType type, GameObject* owner, std::string unique_id);
	virtual ~Component();

	virtual void Start() {};
	virtual void Update() {};
	virtual void CleanUp() {};

	GameObject* GetOwner() const;
	const ComponentType GetType() const;
	const char* GetName() const;

	std::string GetUniqueId();

	void SetActive(const bool& set);
	bool GetActive() const;
	void SetName(const char* new_name);

	void SetOnePerGo(bool set);
	void SetCanDestroy(bool set);
	void SetCanDisable(bool set);

	bool GetOnePerGo() const;
	bool GetCanDestroy() const;
	bool GetCanDisable() const;

	virtual void OnGetBoundingBox(AABB &bbox) {};
	virtual void InspectorDraw(std::vector<Component*> components) {};

	virtual void OnLoadAbstraction(DataAbstraction& config) {};
	virtual void OnSaveAbstraction(DataAbstraction& config) {};

	void ForceUid(std::string uid);

private:
	bool		  active = false;
	GameObject*   owner = nullptr;
	ComponentType type;
	const char*   name = nullptr;
	std::string	  unique_id;

	bool		  one_per_go = false;
	bool		  can_destroy = true;
	bool	      can_disable = true;
};

#endif