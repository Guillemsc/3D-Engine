#ifndef _MODULE_GAME_OBJECT_H_
#define _MODULE_GAME_OBJECT_H_

#include "Module.h"
#include "GameObject.h"
#include "ImGuizmo.h"

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
	void DestroyAllGameObjectsNow();

	const vector<GameObject*> GetListGameObjects() const;

	void AddGameObjectToSelected(GameObject* go);
	void RemoveGameObjectFromSelected(GameObject* go);
	void ClearSelection();
	void DestroySelectedGameObjects();
	const vector<GameObject*> GetSelectedGameObjects() const;
 
	GameObject* GetRoot();

	void AddGameObjectToStatic(GameObject* go);
	void RemoveGameObjectFromStatic(GameObject* go);
	const vector<GameObject*> GetStaticGameObjects() const;
	void AddGameObjectToStaticRecursive(GameObject* go);
	void RemoveGameObjectFromStaticRecursive(GameObject* go);

	const vector<GameObject*> GetDynamicGameObjects() const;

	void SetGuizmoOperation(ImGuizmo::OPERATION op);

	KDTree* GetKDTree();
	void RecalculateKDTree();
	void SetDebugKDTree(bool set);
	void SetDebugBBoxes(bool set);

	void RecursiveGetGameObjectTree(GameObject* go, vector<GameObject*>& fill);

private:
	void DestroyGameObjects();
	void MousePick();
	void UpdateKDTree();

private:
	vector<GameObject*> game_objects;
	list<GameObject*>   to_delete;
	vector<GameObject*> selected;
	vector<GameObject*> statics;
	GameObject*			root = nullptr;

	KDTree*				kdtree = nullptr;
	bool				update_kdtree = false;

	bool				show_kdtree = false;
	bool				show_bboxes = false;

	ImGuizmo::OPERATION current_gizmo_operation = ImGuizmo::OPERATION::TRANSLATE;
	float3 last_scale = float3::zero;
};

#endif // !_MODULE_GAME_OBJECT_H_