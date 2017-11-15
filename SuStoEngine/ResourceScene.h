#ifndef __RESOURCE_SCENE_H__
#define __RESOURCE_SCENE_H__

#include "Resource.h"
#include <vector>

class GameObject;
class Component;

class ResourceScene : public Resource
{
public:
	ResourceScene(std::string unique_id);
	virtual ~ResourceScene();

	void CleanUp();


private:
	void LoadToMemory();
	void UnloadFromMemory();

private:
	std::vector<GameObject> game_objects;
	std::vector<Component> components;

	bool loaded = false;
};

#endif