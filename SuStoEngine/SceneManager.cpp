#include "SceneManager.h"
#include "JSONLoader.h"
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
		config->SetString("scene.title", "Scenes");
		for (list<Module*>::iterator it = App->modules.begin(); it != App->modules.end(); ++it)
		{
			(*it)->SaveScene(config);
		}

		config->Save();
	}
}

void SceneManager::LoadScene(const char * scene_name)
{
}
