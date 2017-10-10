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

	vector<GameObject*> GetListGameObjects();

	void AddGameObjectToSelected(GameObject* go);
	void RemoveGameObjectFromSelected(GameObject* go);
	void ClearSelection();

	vector<GameObject*> GetSelectedGameObjects();

private:
	void DestroyGameObjects();

private:
	vector<GameObject*> game_objects;
	vector<GameObject*> to_delete;
	vector<GameObject*> selected;

};

#endif // !_MODULE_GAME_OBJECT_H_