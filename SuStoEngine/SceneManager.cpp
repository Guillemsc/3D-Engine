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
	config = App->json->CreateJSON(scene_name);

	if (config)
	{
		vector<GameObject*> game_objects = App->gameobj->GetListGameObjects();

		// Store GameObjects
		config->SetArray("GameObjects");
		config->SetArray("Components");

		for (vector<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end(); it++)
		{
			(*it)->OnSaveScene(config);
		}

		config->Save();
	}
}

void SceneManager::LoadScene(const char * scene_name)
{
}

void SceneManager::RecursiveSaveGameObject(GameObject * go)
{

}
