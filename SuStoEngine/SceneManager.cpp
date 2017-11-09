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
			scene->MoveToRoot();
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
		// Load GameObjects
		int game_objects_count = scene->GetArrayCount("GameObjects");
		for (int i = 0; i < game_objects_count; i++)
		{
			scene->MoveToSectionFromArray("GameObjects", i);

			string id = scene->GetString("uid");
			string name = scene->GetString("name");

			GameObject* go = App->gameobj->Create(id);
			go->SetName(name);

			scene->MoveToRoot();
		}

		// Load Components
		int components_count = scene->GetArrayCount("Components");
		for (int i = 0; i < components_count; i++)
		{
			scene->MoveToSectionFromArray("Components", i);

			ComponentType type = static_cast<ComponentType>((int)scene->GetNumber("type", 0));
			string component_id = scene->GetString("component_id", "no_id");
			string owner_id = scene->GetString("owner_id", "no_id");
			GameObject* owner = App->gameobj->Find(owner_id);

			switch (type)
			{
			case TRANSFORM:
			{
				float3 position = scene->GetNumber3("position");
				float4 rotation = scene->GetNumber4("rotation");
				float3 scale = scene->GetNumber3("scale");
				owner->transform->SetPosition(position);
				owner->transform->SetRotation(Quat(rotation.x, rotation.y, rotation.w, rotation.z));
				owner->transform->SetScale(scale);
			}
			break;
			}

			scene->MoveToRoot();
		}
	}
}

void SceneManager::RecursiveSaveGameObject(GameObject * go)
{

}
