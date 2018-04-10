#ifndef __RESOURCE_PREFAB_H__
#define __RESOURCE_PREFAB_H__

#include "Resource.h"
#include <string>
#include "Globals.h"
#include "GeometryMath.h"
#include "GameObject.h"

class ResourcePrefab : public Resource
{
public:
	ResourcePrefab(std::string unique_id);
	virtual ~ResourcePrefab();

	void CleanUp();

private:

private:
	GameObject* base_go = nullptr;
};

#endif //!__RESOURCE_PREFAB_H__