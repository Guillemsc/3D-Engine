#ifndef __SCENE_MANAGER_H__
#define __SCENE_MANAGER_H__

#include "Module.h"

class GameObject;

class SceneManager : public Module
{
public:
	SceneManager(bool start_enabled = true);
	virtual ~SceneManager();

	void SaveScene(const char* scene_name);
	void LoadScene(const char* scene_name);

private:
	void RecursiveSaveGameObject(GameObject* go);

public:

private:
	JSON_Doc* config = nullptr;
};

#endif