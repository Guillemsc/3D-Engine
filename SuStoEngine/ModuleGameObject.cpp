#include "ModuleGameObject.h"
#include "App.h"
#include "IDGenerator.h"

ModuleGameObject::ModuleGameObject(bool enabled)
{
	SetName("GameObject");
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

	for (vector<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end(); ++it)
	{
		Destroy((*it));
	}

	DestroyGameObjects();

	return ret;
}

GameObject * ModuleGameObject::Create()
{
	GameObject* game_object = new GameObject(App->id->NewId("gameobject"));

	game_objects.push_back(game_object);
	game_object->Start();

	return game_object;
}

void ModuleGameObject::Destroy(GameObject * go)
{
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

const vector<GameObject*> ModuleGameObject::GetSelectedGameObjects() const
{
	return selected;
}

void ModuleGameObject::DestroyGameObjects()
{
	for (vector<GameObject*>::iterator to_del = to_delete.begin(); to_del != to_delete.end();)
	{
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

		RemoveGameObjectFromSelected(*to_del);

		(*to_del)->CleanUp();
		delete (*to_del);

		to_del = to_delete.erase(to_del);
	}
}
