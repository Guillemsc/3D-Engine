#include "ModuleGameObject.h"

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

	for (list<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end(); ++it)
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

	for (list<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end(); ++it)
	{
		Destroy((*it));
	}
	game_objects.clear();

	DestroyGameObjects();

	return ret;
}

GameObject * ModuleGameObject::Create()
{
	GameObject* game_object = new GameObject();

	game_object->SetId(game_objects.size());
	game_objects.push_back(game_object);

	return game_object;
}

void ModuleGameObject::Destroy(GameObject * go)
{
	to_delete.push_back(go);
}

list<GameObject*> ModuleGameObject::GetListGameObjects()
{
	return game_objects;
}

void ModuleGameObject::DestroyGameObjects()
{
	for (list<GameObject*>::iterator to_del = to_delete.begin(); to_del != to_delete.end();)
	{
		for (list<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end();)
		{
			if ((*to_del) == (*it))
			{
				it = game_objects.erase(it);
				break;
			}
			else
				++it;
		}


		(*to_del)->CleanUp();
		delete (*to_del);

		to_del = to_delete.erase(to_del);
	}
}
