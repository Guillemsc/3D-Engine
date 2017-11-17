#include "ModuleGameObject.h"
#include "App.h"
#include "ModuleRenderer3D.h"
#include "ResourceMeshLoader.h"
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
#include "ImGuizmo.h"
#include "imgui.h"

ModuleGameObject::ModuleGameObject(bool enabled)
{
	SetName("GameObject");

	// KDTree
	kdtree = new KDTree();

	// Root GameObject
	root = new GameObject("Root", this);
	root->Start();
	root->SetName("Root");
}

ModuleGameObject::~ModuleGameObject()
{
}

bool ModuleGameObject::Awake()
{
	bool ret = true;

	LOG_OUTPUT("Creating Module GameObject");	

	return ret;
}

bool ModuleGameObject::Start()
{
	bool ret = true;

	ResourceMeshLoader loader;
	loader.CreatePlane();

	return ret;
}

bool ModuleGameObject::PreUpdate()
{
	bool ret = true;

	root->RecursiveCalcGlobalTransform();
	root->RecursiveCalcBBox();

	UpdateKDTree();

	return ret;
}

bool ModuleGameObject::Update()
{
	bool ret = true;

	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		MousePick();
	}

	vector<Camera3D*> cameras = App->camera->GetCameras();
	vector<GameObject*> to_draw;

	// Get elements to draw from all cameras
	for (vector<Camera3D*>::iterator it = cameras.begin(); it != cameras.end(); ++it)
	{
		if((*it)->GetFrustumCulling())
			(*it)->GetElementsToDraw(to_draw);
	}
	
	// Draw
	for(vector<GameObject*>::iterator it = to_draw.begin(); it != to_draw.end(); ++it)
		(*it)->Draw();
	
	// Update
	for (vector<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end(); ++it)
	{
		(*it)->UpdateComponents();

		if (App->scene_manager->GetState() == SceneState::PLAY)
			(*it)->UpdateLogic();

		(*it)->SetDebugDraw(show_bboxes);
	}

	for (vector<GameObject*>::iterator it = selected.begin(); it != selected.end(); ++it)
	{
		float4x4 transform = (*it)->transform->GetGlobalTransform().Transposed();

		float transformation[16];
		ImGuizmo::Manipulate(App->camera->GetCurrentCamera()->GetOpenGLViewMatrix().ptr(),
			App->camera->GetCurrentCamera()->GetOpenGLProjectionMatrix().ptr(),
			current_gizmo_operation,
			ImGuizmo::MODE::WORLD,
			transform.ptr(), transformation);

		if (ImGuizmo::IsUsing())
		{
			float addition[3];
			float rotation[3];
			float scale[3];
			ImGuizmo::DecomposeMatrixToComponents(transformation, addition, rotation, scale);
			float3 add(addition[0], addition[1], addition[2]);
			float3 rot(rotation[0], rotation[1], rotation[2]);
			float3 sc(scale[0], scale[1], scale[2]);

			switch (current_gizmo_operation)
			{
				case ImGuizmo::OPERATION::TRANSLATE:
				{
					if (add.IsFinite()) 
					{
						if ((*it)->parent != nullptr) 
						{
							add = (*it)->parent->transform->GetGlobalTransform().Inverted().TransformPos(add);
						}

						(*it)->transform->Translate(add);
					}		
				}
				break;
				case ImGuizmo::OPERATION::ROTATE:
				{
					if (rot.IsFinite()) 
					{
						if ((*it)->parent != nullptr) 
						{
							rot = (*it)->parent->transform->GetGlobalTransform().Inverted().TransformPos(rot);
						}
						(*it)->transform->Rotate(rot);
					}				
				}
				break;
				case ImGuizmo::OPERATION::SCALE:
				{
					if (sc.IsFinite()) 
					{
						(*it)->transform->SetScale(sc);
					}
					
				}
				break;
			}
		}
	}

	if (show_kdtree)
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

	kdtree->CleanUp();
	RELEASE(kdtree);

	return ret;
}

GameObject * ModuleGameObject::Create(std::string force_id)
{
	string new_id;

	if (force_id == "")
		new_id = GetUIDRandomHexadecimal();
	else
		new_id = force_id;

	GameObject* game_object = new GameObject(new_id, this);

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
	if (go->GetSelected())
		return;

	for (vector<GameObject*>::iterator it = selected.begin(); it != selected.end(); ++it)
	{
		if ((*it) == go)
			return;
	}

	selected.push_back(go);
	go->selected = true;
}

void ModuleGameObject::RemoveGameObjectFromSelected(GameObject * go)
{
	if (!go->GetSelected())
		return;

	for (vector<GameObject*>::iterator it = selected.begin(); it != selected.end(); ++it)
	{
		if ((*it) == go)
		{
			go->selected = false,
			selected.erase(it);
			return;
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
	if (go->GetStatic())
		return;
	
	for (vector<GameObject*>::iterator it = statics.begin(); it != statics.end(); it++)
	{
		if ((*it) == go)
		{
			break;
		}
	}

	statics.push_back(go);
	go->is_static = true;

	RecalculateKDTree();
}

void ModuleGameObject::RemoveGameObjectFromStatic(GameObject * go)
{
	if (!go->GetStatic())
		return;

	for (vector<GameObject*>::iterator it = statics.begin(); it != statics.end(); it++)
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
		kdtree->GetElementsToTest(picking, App->camera->GetCurrentCamera()->GetNearPlaneDistance(), App->camera->GetCurrentCamera()->GetFarPlaneDistance(), gos);

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

