#include "ModuleGameObject.h"
#include "App.h"
#include "IDGenerator.h"

ModuleGameObject::ModuleGameObject(bool enabled)
{
}

ModuleGameObject::~ModuleGameObject()
{
}

bool ModuleGameObject::Awake()
{
	bool ret = true;

	LOG_OUTPUT("Creating Module GameObject");
	for (int i = 0; i < 250; ++i) {
		selected_go[i] = false;
		all_false[i] = false;
	}


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
	game_objects.clear();

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

vector<GameObject*> ModuleGameObject::GetListGameObjects()
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

vector<GameObject*> ModuleGameObject::GetSelectedGameObjects()
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
