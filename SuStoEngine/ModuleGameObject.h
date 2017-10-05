#ifndef _MODULE_GAME_OBJECT_H_
#define _MODULE_GAME_OBJECT_H_

#include "Module.h"
#include "GameObject.h"

class ModuleGameObject : public Module
{
public:
	ModuleGameObject(bool enabled = true);
	~ModuleGameObject();

	bool Awake();
	bool PreUpdate();
	bool Update();
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	GameObject* Create();
	void Destroy(GameObject* go);

private:
	void DestroyGameObjects();

public:

private:
	list<GameObject*> game_objects;
	list<GameObject*> to_delete;

};

#endif // !_MODULE_GAME_OBJECT_H_