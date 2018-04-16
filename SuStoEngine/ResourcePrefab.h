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
	void Instantiate(GameObject* parent = nullptr);

private:

private:
	GameObjectAbstraction abstraction;
};

#endif //!__RESOURCE_PREFAB_H__