#include "ModuleGameObject.h"
#include "App.h"
#include "ModuleRenderer3D.h"
#include "DebugDraw.h"
#include "IDGenerator.h"
#include "Functions.h"

ModuleGameObject::ModuleGameObject(bool enabled)
{
	SetName("GameObject");

	// Root GameObject
	root = new GameObject(-1);
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

	for (vector<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end(); ++it)
	{
		(*it)->Update();
		(*it)->Draw();
	}

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

GameObject * ModuleGameObject::Create(double id)
{
	GameObject* game_object = new GameObject(id);

	game_objects.push_back(game_object);
	root->AddChild(game_object);
	game_object->Start();

	return game_object;
}

GameObject * ModuleGameObject::Create()
{
	return Create(GetUniqueIdentifierRandom());
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
