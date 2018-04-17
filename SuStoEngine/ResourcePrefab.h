#ifndef __RESOURCE_PREFAB_H__
#define __RESOURCE_PREFAB_H__

#include "Resource.h"
#include <string>
#include "GeometryMath.h"
#include "GameObjectAbstractor.h"

class GameObject;

class ResourcePrefab : public Resource
{
public:
	ResourcePrefab(std::string unique_id);
	virtual ~ResourcePrefab();

	void CleanUp();

	void SetGameObject(GameObject* go);
	void SetAbstraction(const GameObjectAbstraction& abs);
	void Instantiate(GameObject* parent = nullptr);

	GameObjectAbstraction GetAbstraction() const;

private:

private:
	GameObjectAbstraction abstraction;
};

#endif //!__RESOURCE_PREFAB_H__