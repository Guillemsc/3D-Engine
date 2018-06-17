#include "ModuleGameObject.h"
#include "App.h"
#include "ModuleRenderer3D.h"
#include "ResourceMeshLoader.h"
#include "ResourceManager.h"
#include "DebugDraw.h"
#include "IDGenerator.h"
#include "Functions.h"
#include "KDTree.h"
#include "ModuleInput.h"
#include "SceneManager.h"
#include "ModuleCamera3D.h"
#include "EditorUI.h"
#include "ModuleWindow.h"
#include "ComponentCamera.h"
#include "ComponentMesh.h"
#include "ResourceMesh.h"
#include "ComponentMaterial.h"
#include "ImGuizmo.h"
#include "imgui.h"
#include "Resource.h"
#include "ResourceTextureLoader.h"
#include "ModuleEventSystem.h"

ModuleGameObject::ModuleGameObject(bool enabled) : Module(enabled)
{
	SetName("GameObject");
}

ModuleGameObject::~ModuleGameObject()
{
}

bool ModuleGameObject::Awake()
{
	bool ret = true;

	CONSOLE_LOG("Creating Module GameObject");	

	App->event_system->Suscribe(EventType::ET_GAMEOBJECT_DESTROY, GameObjectEvents);
	App->event_system->Suscribe(EventType::ET_GAMEOBJECT_CHANGE_PARENT, GameObjectEvents);

	// KDTree
	kdtree = new KDTree();
	go_abstractor = new GameObjectAbstractor();

	// Root GameObject
	root = new GameObject("Root");
	root->Start();
	root->SetName("Root");

	return ret;
}

bool ModuleGameObject::Start()
{
	bool ret = true;

	return ret;
}

bool ModuleGameObject::PreUpdate()
{
	bool ret = true;

	root->RecursiveCalcGlobalTransform();
	root->RecursiveCalcBBox();

	UpdateKDTree();

	float x = App->editorUI->GameRect().left;
	float y = App->editorUI->GameRect().top;
	float w = App->editorUI->GameRect().right - App->editorUI->GameRect().left;
	float h = App->editorUI->GameRect().bottom - App->editorUI->GameRect().top;

	GameObjectsPreUpdate();

	return ret;
}

bool ModuleGameObject::Update()
{
	bool ret = true;

	if (App->input->GetMouseButton(1) && can_pick)
		MousePick();

	vector<Camera3D*> cameras = App->camera->GetCameras();

	// Get elements to draw from all cameras
	App->camera->GetEditorCamera()->GetElementsToDraw();
	
	// Update
	GameObjectsUpdate();

	UpdateTransformationGuizmos();
	
	if (show_kdtree)
		kdtree->DebugDraw();

	/*App->renderer3D->DrawGrid(100);*/

	return ret;
}

bool ModuleGameObject::PostUpdate()
{
	bool ret = true;

	GameObjectsPostUpdate();

	DestroyGameObjects();

	return ret;
}

bool ModuleGameObject::CleanUp()
{
	bool ret = true;
 
	Destroy(root);

	DestroyGameObjects();

	kdtree->CleanUp();
	RELEASE(kdtree);

	RELEASE(go_abstractor);

	return ret;
}

void ModuleGameObject::GameObjectsPreUpdate()
{
	for (vector<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end(); ++it)
	{
		(*it)->PreUpdate();
	}
}

void ModuleGameObject::GameObjectsUpdate()
{
	// Update
	for (vector<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end(); ++it)
	{
		(*it)->UpdateComponents();

		//if (App->scene_manager->GetState() == SceneState::PLAY)
		//	(*it)->UpdateLogic();

		//(*it)->Draw();

		//(*it)->SetDebugDraw(show_bboxes);
	}
}

void ModuleGameObject::GameObjectsPostUpdate()
{
	for (vector<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end(); ++it)
	{
		(*it)->PostUpdate();

		// Last
		(*it)->DestroyComponents();
	}
}

GameObject * ModuleGameObject::Create(std::string id)
{
	GameObject* game_object = new GameObject(id);

	bool can_add = AddGO(game_object);

	if (can_add)
	{
		game_object->SetParent(root);
		game_object->Start();

		Event ev(EventType::ET_GAMEOBJECT_CREATE);
		ev.game_object_create.go = game_object;
		App->event_system->Send(ev);
	}
	else
	{
		game_object->CleanUp();
		delete game_object;

		game_object = nullptr;
	}

	return game_object;
}

GameObject * ModuleGameObject::Create()
{
	string new_id = App->resource_manager->GetNewUID();
	return Create(new_id);
}

bool ModuleGameObject::AddGO(GameObject * go)
{
	bool ret = false;

	if (go != nullptr)
	{
		bool exists = false;
		for (std::vector<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end(); ++it)
		{
			if ((*it)->GetUniqueId() == go->GetUniqueId() || go == (*it))
			{
				exists = true;
				break;
			}
		}

		if (!exists)
		{
			game_objects.push_back(go);
			ret = true;
		}
	}

	return ret;
}

void ModuleGameObject::Destroy(GameObject * go)
{
	if (go != nullptr)
	{
		for (list<GameObject*>::iterator it = to_delete.begin(); it != to_delete.end(); ++it)
		{
			if (go == (*it))
				return;
		}

		Event ev(EventType::ET_GAMEOBJECT_DESTROY);
		ev.game_object_destroy.go = go;
		App->event_system->Send(ev);

		to_delete.push_back(go);
	}
}

GameObject* ModuleGameObject::Find(std::string unique_id)
{
	GameObject* ret = nullptr;

	for (vector<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end(); ++it)
	{
		if ((*it)->GetUniqueId() == unique_id)
		{
			ret = (*it);
			break;
		}
	}

	return ret;
}

GameObject * ModuleGameObject::Duplicate(GameObject * go)
{
	GameObject * ret = nullptr;

	if (go != nullptr)
	{
		GameObjectAbstraction abs = GetAbstractor()->Abstract(go);
		ret = GetAbstractor()->DeAbstract(abs);
	}

	return ret;
}

void ModuleGameObject::DestroyAllGameObjects()
{
	for (vector<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end(); ++it)
	{
		Destroy((*it));
	}
}

void ModuleGameObject::DestroyAllGameObjectsNow()
{
	DestroyAllGameObjects();

	DestroyGameObjects();
}

const vector<GameObject*> ModuleGameObject::GetListGameObjects() const
{
	return game_objects;
}

void ModuleGameObject::AddGameObjectToSelected(GameObject * go)
{
	if (go != nullptr)
	{
		if (!go->GetSelected())
		{
			for (vector<GameObject*>::iterator it = selected.begin(); it != selected.end(); ++it)
			{
				if ((*it) == go)
					return;
			}

			selected.push_back(go);
			go->selected = true;
		}
	}
}

void ModuleGameObject::RemoveGameObjectFromSelected(GameObject * go)
{
	if (go != nullptr)
	{
		if (go->GetSelected())
		{
			for (vector<GameObject*>::iterator it = selected.begin(); it != selected.end(); ++it)
			{
				if ((*it) == go)
				{
					go->selected = false;
					selected.erase(it);

					return;
				}
			}
		}
	}
}

void ModuleGameObject::ClearSelection()
{
	for (vector<GameObject*>::iterator it = selected.begin(); it != selected.end();)
	{
		(*it)->selected = false;
		it = selected.erase(it);
	}
}

void ModuleGameObject::DestroySelectedGameObjects()
{
	for (vector<GameObject*>::iterator it = selected.begin(); it != selected.end(); it++)
	{
		App->gameobj->Destroy((*it));
	}

	ClearSelection();
}

GameObject * ModuleGameObject::GetRoot()
{
	return root;
}

const vector<GameObject*> ModuleGameObject::GetSelectedGameObjects() const
{
	return selected;
}

void ModuleGameObject::AddGameObjectToStatic(GameObject * go)
{
	if (go != nullptr)
	{
		if (go->GetStatic())
			return;

		for (vector<GameObject*>::iterator it = statics.begin(); it != statics.end(); ++it)
		{
			if ((*it) == go)
			{
				return;
			}
		}

		statics.push_back(go);
		go->is_static = true;

		RecalculateKDTree();
	}
}

void ModuleGameObject::RemoveGameObjectFromStatic(GameObject * go)
{
	if (!go->GetStatic())
		return;

	for (vector<GameObject*>::iterator it = statics.begin(); it != statics.end(); ++it)
	{
		if ((*it) == go)
		{
			statics.erase(it);
			go->is_static = false;
			break;
		}
	}

	RecalculateKDTree();
}

const vector<GameObject*> ModuleGameObject::GetStaticGameObjects() const
{
	return statics;
}

void ModuleGameObject::AddGameObjectToStaticRecursive(GameObject * go)
{
	AddGameObjectToStatic(go);

	vector<GameObject*> childs = go->GetChilds();
	for (vector<GameObject*>::iterator it = childs.begin(); it != childs.end(); ++it)
	{
		AddGameObjectToStaticRecursive(*it);
	}
}

void ModuleGameObject::RemoveGameObjectFromStaticRecursive(GameObject * go)
{
	RemoveGameObjectFromStatic(go);

	vector<GameObject*> childs = go->GetChilds();
	for (vector<GameObject*>::iterator it = childs.begin(); it != childs.end(); ++it)
	{
		RemoveGameObjectFromStaticRecursive(*it);
	}
}

const vector<GameObject*> ModuleGameObject::GetDynamicGameObjects() const
{
	vector<GameObject*> dynamics;

	for (vector<GameObject*>::const_iterator it = game_objects.begin(); it != game_objects.end(); it++)
	{
		if (!(*it)->is_static)
			dynamics.push_back((*it));
	}

	return dynamics;
}

void ModuleGameObject::SetGuizmoOperation(ImGuizmo::OPERATION op)
{
	current_gizmo_operation = op;
}

ImGuizmo::OPERATION ModuleGameObject::GetGuizmoOperation() const
{
	return current_gizmo_operation;
}

void ModuleGameObject::SetGuizmoMode(ImGuizmo::MODE mode)
{
	curr_guizmo_mode = mode;
}

ImGuizmo::MODE ModuleGameObject::GetGuizmoMode()
{
	return curr_guizmo_mode;
}

KDTree * ModuleGameObject::GetKDTree()
{
	return kdtree;
}

void ModuleGameObject::RecalculateKDTree()
{
	update_kdtree = true;
}

void ModuleGameObject::SetDebugKDTree(bool set)
{
	show_kdtree = set;
}

void ModuleGameObject::SetDebugBBoxes(bool set)
{
	show_bboxes = set;
}

void ModuleGameObject::RecursiveGetGameObjectTree(GameObject * go, vector<GameObject*>& fill)
{
	fill.push_back(go);

	vector<GameObject*> childs = go->GetChilds();
	for (vector<GameObject*>::iterator it = childs.begin(); it != childs.end(); ++it)
	{
		RecursiveGetGameObjectTree(*it, fill);
	}
}

void ModuleGameObject::SetCanPick(bool set)
{
	can_pick = set;
}

void ModuleGameObject::SetCanMove(bool set)
{
	can_move = set;
}

GameObjectAbstractor * ModuleGameObject::GetAbstractor() const
{
	return go_abstractor;
}

void ModuleGameObject::DestroyGameObjects()
{
	for (list<GameObject*>::iterator to_del = to_delete.begin(); to_del != to_delete.end();)
	{
		// Add childs to delete and reset parent
		vector<GameObject*> childs = (*to_del)->GetChilds();
		for (vector<GameObject*>::iterator ch = childs.begin(); ch != childs.end();)
		{
			(*ch)->parent = nullptr;

			Destroy(*ch);

			ch = childs.erase(ch);
		}
		
		// Reset parent
		if ((*to_del)->GetParent() != nullptr)
		{
			GameObject* parent = (*to_del)->GetParent();

			if (parent != nullptr)
			{
				for (vector<GameObject*>::iterator ch = parent->childs.begin(); ch != parent->childs.end(); ++ch)
				{
					if ((*ch) == (*to_del))
					{
						parent->childs.erase(ch);
						break;
					}
				}

				(*to_del)->parent = nullptr;
			}
		}

		// Delete from list
		for (vector<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end(); ++it)
		{
			if ((*to_del) == (*it))
			{
				game_objects.erase(it);
				break;
			}
		}

		// Delete from selected
		RemoveGameObjectFromSelected(*to_del);
		RemoveGameObjectFromStatic(*to_del);

		// Free
		(*to_del)->CleanUp();
		delete (*to_del);

		to_del = to_delete.erase(to_del);
	}
}

void ModuleGameObject::MousePick()
{
	Rect window = App->editorUI->GameRect();
	float2 mouse_pos = App->input->GetMouse();

	if (PointInRect(mouse_pos, window))
	{
		 //The point (1, 1) corresponds to the top-right corner of the near plane
		 //(-1, -1) is bottom-left

		float first_normalized_x = (mouse_pos.x - window.left) / (window.right - window.left);
		float first_normalized_y = (mouse_pos.y - window.top) / (window.bottom - window.top);

		float normalized_x = (first_normalized_x * 2) - 1;
		float normalized_y = 1 - (first_normalized_y * 2);

		LineSegment picking = App->camera->GetCurrentCamera()->GetFrustum().UnProjectLineSegment(normalized_x, normalized_y);

		float distance = 99999999999;
		GameObject* closest = nullptr;

		vector<GameObject*> gos = App->gameobj->GetDynamicGameObjects();
		kdtree->GetElementsToTest(picking, 0, App->camera->GetCurrentCamera()->GetFarPlaneDistance(), gos);

		for (vector<GameObject*>::iterator it = gos.begin(); it != gos.end(); it++)
		{
			bool hit;
			float dist;
			(*it)->TestRay(picking, hit, dist);

			if (hit)
			{
				if (dist < distance)
				{
					distance = dist;
					closest = (*it);
				}
			}
		}

		if (closest != nullptr)
		{
			ClearSelection();
			AddGameObjectToSelected(closest);
		}
	}
}

void ModuleGameObject::UpdateKDTree()
{
	if (update_kdtree)
	{
		update_kdtree = false;
		kdtree->CreateTree(statics);
	}
}

void ModuleGameObject::UpdateTransformationGuizmos()
{
	ImGuizmo::MODE mode_to_apply = curr_guizmo_mode;

	if (current_gizmo_operation == ImGuizmo::OPERATION::SCALE)
		mode_to_apply = ImGuizmo::MODE::WORLD;

	for (vector<GameObject*>::iterator it = selected.begin(); it != selected.end(); ++it)
	{
		float4x4 global_transform_trans = (*it)->transform->GetGlobalTransform().Transposed();
		float t[16];

		ImGuizmo::Manipulate(App->camera->GetCurrentCamera()->GetOpenGLViewMatrix().ptr(),
			App->camera->GetCurrentCamera()->GetOpenGLProjectionMatrix().ptr(),
			current_gizmo_operation,
			mode_to_apply,
			global_transform_trans.ptr(), t);

		float4x4 moved_transformation = float4x4(
			t[0], t[4], t[8], t[12],
			t[1], t[5], t[9], t[13],
			t[2], t[6], t[10], t[14],
			t[3], t[7], t[11], t[15]);

		if (ImGuizmo::IsUsing() && can_move)
		{
			switch (current_gizmo_operation)
			{
			case ImGuizmo::OPERATION::TRANSLATE:
			{
				float4x4 new_trans = moved_transformation * (*it)->transform->GetGlobalTransform();
				(*it)->transform->SetGlobalTransform(new_trans);
			}
			break;

			case ImGuizmo::OPERATION::ROTATE:
			{
				float4x4 new_trans = moved_transformation * (*it)->transform->GetGlobalTransform();
				(*it)->transform->SetGlobalTransform(new_trans);
			}
			break;
			case ImGuizmo::OPERATION::SCALE:
			{
				float4x4 save_trans = moved_transformation;
				moved_transformation = moved_transformation * last_moved_transformation.Inverted();

				float4x4 new_trans = moved_transformation * (*it)->transform->GetGlobalTransform();
				(*it)->transform->SetGlobalTransform(new_trans);

				last_moved_transformation = save_trans;
			}
			break;
			}
		}
		else
		{
			last_moved_transformation = float4x4::identity;
		}
	}

	if (ImGuizmo::IsOver() || ImGuizmo::IsUsing())
	{
		can_pick = false;
	}
	else
	{
		can_pick = true;
	}
}

void GameObjectEvents(Event ev)
{
}
