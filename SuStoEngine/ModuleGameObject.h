#ifndef _MODULE_GAME_OBJECT_H_
#define _MODULE_GAME_OBJECT_H_

#include "Module.h"
#include "GameObject.h"

class KDTree;

class ModuleGameObject : public Module
{
public:
	ModuleGameObject(bool enabled = true);
	~ModuleGameObject();

	bool Awake();
	bool Start();
	bool PreUpdate();
	bool Update();
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	GameObject* Create(std::string unique_id = "");
	void Destroy(GameObject* go);
	GameObject* Find(std::string unique_id);
	void DestroyAllGameObjects();

	const vector<GameObject*> GetListGameObjects() const;

	void AddGameObjectToSelected(GameObject* go);
	void RemoveGameObjectFromSelected(GameObject* go);
	void ClearSelection();
	void DestroySelectedGameObjects();
 
	GameObject* GetRoot();

	const vector<GameObject*> GetSelectedGameObjects() const;

private:
	void DestroyGameObjects();

private:
	vector<GameObject*> game_objects;
	list<GameObject*>   to_delete;
	vector<GameObject*> selected;
	vector<GameObject*> statics;
	GameObject*			root = nullptr;
	KDTree* kdtree = nullptr;
	bool kdtree_debug = false;
};

#endif // !_MODULE_GAME_OBJECT_H_