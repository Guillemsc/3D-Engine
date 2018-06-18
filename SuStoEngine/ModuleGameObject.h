#ifndef __MODULE_GAME_OBJECT_H__
#define __MODULE_GAME_OBJECT_H__

#include "Module.h"
#include "GameObject.h"
#include "ImGuizmo.h"

class KDTree;
class Resource;
struct TextureInfo;
class Event;

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
	bool CleanUp();

	void GameObjectsPreUpdate();
	void GameObjectsUpdate();
	void GameObjectsPostUpdate();

	GameObject* Create(std::string id);
	GameObject* Create();
	bool AddGO(GameObject* go);

	GameObject* Find(std::string unique_id);

	GameObject* Duplicate(GameObject* go);

	void Destroy(GameObject* go);
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
	ImGuizmo::OPERATION GetGuizmoOperation() const;
	void SetGuizmoMode(ImGuizmo::MODE mode);
	ImGuizmo::MODE GetGuizmoMode();

	KDTree* GetKDTree();
	void RecalculateKDTree();
	void SetDebugKDTree(bool set);
	void SetDebugBBoxes(bool set);

	void RecursiveGetGameObjectTree(GameObject* go, vector<GameObject*>& fill);

	void SetCanPick(bool set);
	void SetCanMove(bool set);

	GameObjectAbstractor* GetAbstractor() const;

private:
	void DestroyGameObjects();
	void MousePick();
	void UpdateKDTree();
	void UpdateTransformationGuizmos();

private:
	vector<GameObject*>   game_objects;
	list<GameObject*>     to_delete;
	vector<GameObject*>   selected;
	vector<GameObject*>   statics;
	GameObject*			  root = nullptr;
						  
	KDTree*				  kdtree = nullptr;
	bool				  update_kdtree = false;
						  
	bool				  show_kdtree = false;
	bool				  show_bboxes = false;
						  
	ImGuizmo::OPERATION   current_gizmo_operation = ImGuizmo::OPERATION::TRANSLATE;
	float3 last_scale =   float3::zero;
						  
	bool			      can_pick = true;
	bool			      can_move = true;

	GameObjectAbstractor* go_abstractor = nullptr;

	ImGuizmo::MODE        curr_guizmo_mode = ImGuizmo::MODE::LOCAL;

	float4x4			  last_moved_transformation = float4x4::identity;
};

void OnAddComponent(Event ev);

#endif // !__MODULE_GAME_OBJECT_H__