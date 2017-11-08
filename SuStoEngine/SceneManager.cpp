#include "SceneManager.h"
#include "JSONLoader.h"
#include "GameObject.h"
#include "ModuleGameObject.h"
#include "App.h"

SceneManager::SceneManager(bool start_enabled) : Module(start_enabled)
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::SaveScene(const char * scene_name)
{
	JSON_Doc* scene = App->json->LoadJSON(scene_name);

	if (scene == nullptr)
		scene = App->json->CreateJSON(scene_name);

	if (scene != nullptr)
	{
		scene->Clear();

		vector<GameObject*> game_objects = App->gameobj->GetListGameObjects();

		// Store GameObjects
		scene->SetArray("GameObjects");
		scene->SetArray("Components");

		for (vector<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end(); it++)
		{
			(*it)->OnSaveScene(scene);
		}

		scene->Save();
	}
}

void SceneManager::LoadScene(const char * scene_name)
{
	JSON_Doc* scene = App->json->LoadJSON(scene_name);

	if (scene != nullptr)
	{
		for (int i = 0; i < scene->GetArrayCount("GameObjects"); i++)
		{
			scene->MoveToSectionFromArray("GameObjects", i);

			double id = scene->GetNumber("uid");

			GameObject* go = App->gameobj->Create(id);
		}
	}
}

void SceneManager::RecursiveSaveGameObject(GameObject * go)
{

}
