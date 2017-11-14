#include "ModuleGameObject.h"
#include "App.h"
#include "ModuleRenderer3D.h"
#include "DebugDraw.h"
#include "IDGenerator.h"
#include "Functions.h"
#include "KDTree.h"
#include "ModuleInput.h"
#include "SceneManager.h"
#include "ModuleCamera3D.h"
#include "EditorUI.h"
#include "ImGuizmo.h"
#include "imgui.h"

ModuleGameObject::ModuleGameObject(bool enabled)
{
	SetName("GameObject");

	// Root GameObject
	root = new GameObject("Root");
	root->Start();
	root->SetName("Root");
}

ModuleGameObject::~ModuleGameObject()
{
}

bool ModuleGameObject::Start()
{
	bool ret = true;
 
	return ret;
}

bool ModuleGameObject::Awake()
{
	bool ret = true;

	LOG_OUTPUT("Creating Module GameObject");	

	kdtree = new KDTree();

	return ret;
}

bool ModuleGameObject::PreUpdate()
{
	bool ret = true;

	root->RecursiveCalcGlobalTransform();
	root->RecursiveCalcBBox();

	return ret;
}

bool ModuleGameObject::Update()
{
	bool ret = true;

	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		MousePick();
	}

	for (vector<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end(); ++it)
	{
		(*it)->Update();
		(*it)->Draw();
	}

	for (vector<GameObject*>::iterator it = selected.begin(); it != selected.end(); ++it)
	{
		float4x4 transform = (*it)->transform->GetGlobalTransform().Transposed();

		float3 snap(2, 2, 2);
		ImGuizmo::Manipulate(App->camera->GetCurrentCamera()->GetOpenGLViewMatrix().ptr(),
			App->camera->GetCurrentCamera()->GetOpenGLProjectionMatrix().ptr(),
			current_gizmo_operation,
			current_gizmo_mode,
			transform.ptr(),
			0);

		if (ImGuizmo::IsUsing())
		{
			switch (current_gizmo_operation)
			{
				case ImGuizmo::OPERATION::TRANSLATE:
				{

				}
				break;
			}
		}
	}

	if (App->input->GetKeyDown("K"))
	{
		if (kdtree->HasTree())
			kdtree->EraseTree();

		kdtree->CreateTree(game_objects, 5);;
	}
	
	if (App->input->GetKeyDown("0"))
	{
		kdtree_debug = !kdtree_debug;
	}

	if (kdtree_debug && kdtree != nullptr)
		kdtree->DebugDraw();


	App->renderer3D->DrawGrid(100);

	return ret;
}

bool ModuleGameObject::PostUpdate()
{
	bool ret = true;

	DestroyGameObjects();

	return ret;
}

bool ModuleGameObject::CleanUp()
{
	bool ret = true;
 
	Destroy(root);

	DestroyGameObjects();

	return ret;
}

GameObject * ModuleGameObject::Create(std::string force_id)
{
	string new_id;

	if (force_id == "")
		new_id = GetUIDRandomHexadecimal();
	else
		new_id = force_id;

	GameObject* game_object = new GameObject(new_id);

	game_objects.push_back(game_object);
	root->AddChild(game_object);
	game_object->Start();

	return game_object;
}

void ModuleGameObject::Destroy(GameObject * go)
{
	for (list<GameObject*>::iterator it = to_delete.begin(); it != to_delete.end(); ++it)
	{
		if (go == (*it))
			return;
	}

	to_delete.push_back(go);
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

void ModuleGameObject::DestroyAllGameObjects()
{
	for (vector<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end(); ++it)
	{
		Destroy((*it));
	}

	// Temp
	DestroyGameObjects();
}

const vector<GameObject*> ModuleGameObject::GetListGameObjects() const
{
	return game_objects;
}

void ModuleGameObject::AddGameObjectToSelected(GameObject * go)
{
	for (vector<GameObject*>::iterator it = selected.begin(); it != selected.end(); ++it)
	{
		if ((*it) == go)
			return;
	}

	go->SetSelected(true);
	selected.push_back(go);
}

void ModuleGameObject::RemoveGameObjectFromSelected(GameObject * go)
{
	for (vector<GameObject*>::iterator it = selected.begin(); it != selected.end(); ++it)
	{
		if ((*it) == go)
		{
			go->SetSelected(false);
			selected.erase(it);
			return;
		}
	}
}

void ModuleGameObject::ClearSelection()
{
	for (vector<GameObject*>::iterator it = selected.begin(); it != selected.end();)
	{
		(*it)->SetSelected(false);
		it = selected.erase(it);
	}
}

void ModuleGameObject::DestroySelectedGameObjects()
{
	for (vector<GameObject*>::iterator it = selected.begin(); it != selected.end(); it++)
	{
		App->gameobj->Destroy((*it));
	}
}

GameObject * ModuleGameObject::GetRoot()
{
	return root;
}

const vector<GameObject*> ModuleGameObject::GetSelectedGameObjects() const
{
	return selected;
}

void ModuleGameObject::DestroyGameObjects()
{
	for (list<GameObject*>::iterator to_del = to_delete.begin(); to_del != to_delete.end();)
	{
		// Add childs to delete
		vector<GameObject*> childs = (*to_del)->GetChilds();
		for (vector<GameObject*>::iterator ch = childs.begin(); ch != childs.end(); ++ch)
		{
			(*ch)->SetParentToNull();
			Destroy(*ch);
		}
		
		// Reset parent
		if ((*to_del)->GetParent() != nullptr)
		{
			(*to_del)->GetParent()->EraseChild(*to_del, false);
		}

		// Delete from list
		for (vector<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end();)
		{
			if ((*to_del) == (*it))
			{
				it = game_objects.erase(it);
				break;
			}
			else
				++it;
		}

		// Delete from selected
		RemoveGameObjectFromSelected(*to_del);

		// Free
		(*to_del)->CleanUp();
		delete (*to_del);

		to_del = to_delete.erase(to_del);
	}
}

void ModuleGameObject::MousePick()
{
	float4 rect = App->editorUI->GameRect();
	float2 mouse_pos = App->input->GetMouse();

	if (PointInRect(mouse_pos, rect))
	{
		// The point (1, 1) corresponds to the top-right corner of the near plane
		// (-1, -1) is bottom-left

		float first_normalized_x = (mouse_pos.x - rect.x) / rect.w;
		float first_normalized_y = (mouse_pos.y - rect.y) / rect.z;

		float normalized_x = (first_normalized_x * 2) - 1;
		float normalized_y = 1 - (first_normalized_y * 2);

		LineSegment picking = App->camera->GetCurrentCamera()->GetFrustum().UnProjectLineSegment(normalized_x, normalized_y);

		float distance = 99999999999;
		GameObject* closest = nullptr;
		root->RecursiveTestRay(picking, closest, distance);

		if (closest != nullptr)
		{
			ClearSelection();
			AddGameObjectToSelected(closest);
		}
	}
}

