#include "SceneManager.h"
#include "JSONLoader.h"
#include "GameObject.h"
#include "ModuleGameObject.h"
#include "ResourceManager.h"
#include "ModuleFileSystem.h"
#include "ResourceMesh.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentCamera.h"
#include "ModuleCamera3D.h"
#include "ResourceTexture.h"
#include "ModuleWindow.h"
#include "App.h"

SceneManager::SceneManager(bool start_enabled) : Module(start_enabled)
{
}

SceneManager::~SceneManager()
{
}

bool SceneManager::Start()
{
	bool ret = true;

	current_scene = "test";

	App->resource_manager->ImportAllResources();
	App->scene_manager->LoadScene(current_scene.c_str());

	return ret;
}

bool SceneManager::CleanUp()
{
	bool ret = true;

	App->scene_manager->SaveScene(current_scene.c_str());

	return ret;
}

void SceneManager::SaveScene(const char * scene_name)
{
	SavePrefab(scene_name, "scene", App->file_system->GetLibraryScenePath().c_str(), App->gameobj->GetRoot());
}

void SceneManager::LoadScene(const char * scene_name, bool set_scene_title)
{
	DestroyScene();

	string filepath = App->file_system->GetLibraryScenePath();
	filepath += scene_name;
	filepath += ".scene";

	GameObject* go = nullptr;
	LoadPrefab(filepath.c_str(), go);
}

void SceneManager::DestroyScene()
{
	App->gameobj->ClearSelection();
	App->gameobj->DestroyAllGameObjectsNow();
}

void SceneManager::SaveTmpScene()
{
	SaveScene("tmp_scene");
}

void SceneManager::LoadTmpScene()
{
	LoadScene("tmp_scene", false);
}

bool SceneManager::SavePrefab(const char* name, const char* extension, const char* path, GameObject* go)
{
	bool ret = false;

	LOG_OUTPUT("Saving prefab with name: %s", name);

	string filepath = path;
	filepath += name;
	filepath += ".";
	filepath += extension;

	JSON_Doc* prefab = App->json->LoadJSON(filepath.c_str());

	if (prefab == nullptr)
		prefab = App->json->CreateJSON(filepath.c_str());

	if (prefab != nullptr)
	{
		prefab->Clear();

		vector<GameObject*> game_objects;
		App->gameobj->RecursiveGetGameObjectTree(go, game_objects);

		// Store GameObjects
		prefab->SetArray("GameObjects");

		for (vector<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end(); it++)
		{
			prefab->MoveToRoot();

			if ((*it) != App->gameobj->GetRoot())
			{
				JSON_Doc go_node = prefab->GetJsonNode();

				// Add and move to a new secion on the gameobjects array
				go_node.AddSectionToArray("GameObjects");
				go_node.MoveToSectionFromArray("GameObjects", go_node.GetArrayCount("GameObjects") - 1);

				(*it)->OnSaveSerialize(go_node);
			}
		}

		prefab->Save();

		ret = true;
	}

	return ret;
}

bool SceneManager::LoadPrefab(const char * file_path, GameObject *& loaded_go)
{
	bool ret = false;

	string file_name = App->file_system->GetFileNameFromFilePath(file_path);

	LOG_OUTPUT("Loading prefab with name: %s", file_name.c_str());

	JSON_Doc* prefab = App->json->LoadJSON(file_path);

	if (prefab != nullptr)
	{
		//if (set_scene_title)
		//{
		//	// Engine title --------------
		//	string title_name = App->GetAppName();
		//	title_name += " ";
		//	title_name += App->GetVersion();
		//	title_name += " - ";
		//	title_name += scene_name;
		//	App->window->SetTitle(title_name.c_str());
		//	// ---------------------------
		//}

		// Load GameObjects
		int game_objects_count = prefab->GetArrayCount("GameObjects");
		for (int i = 0; i < game_objects_count; i++)
		{
			JSON_Doc go_node = prefab->GetJsonNode();
			go_node.MoveToSectionFromArray("GameObjects", i);

			string id = go_node.GetString("uid", "no_id");
			GameObject* go = App->gameobj->Create(id);

			if(go != nullptr)
				go->OnLoadSerialize(go_node);

			if (i == 0)
				loaded_go = go;
		}

		// Setting parents and childs
		for (int i = 0; i < game_objects_count; i++)
		{
			JSON_Doc go_node = prefab->GetJsonNode();

			go_node.MoveToSectionFromArray("GameObjects", i);

			string parent = go_node.GetString("parent", "no_id");
			string id = go_node.GetString("uid", "no_id");

			if (parent != "")
			{
				GameObject* go_parent = App->gameobj->Find(parent);
				GameObject* go = App->gameobj->Find(id);

				if(go_parent != nullptr)
					go_parent->AddChild(go);
			}
		}

		prefab->MoveToRoot();
	}

	return ret;
}

SceneState SceneManager::GetState()
{
	return state;
}

bool SceneManager::GetPause()
{
	return pause;
}

bool SceneManager::GetStep()
{
	return step;
}

void SceneManager::Edit()
{
	if (state != EDIT)
	{
		state = EDIT;
		pause = false;
		step = false;

		LoadTmpScene();
	}
}

void SceneManager::Play()
{
	if (state != PLAY)
	{
		state = PLAY;

		SaveTmpScene();
	}
}

void SceneManager::Pause()
{
	pause = false;

	if (state == PLAY)
	{
		pause = true;
	}
}

void SceneManager::Step()
{
	step = false;

	if (state == PLAY)
	{
		step = true;
	}
}

float SceneManager::GetGameDT()
{
	float ret = 0.0f;

	if (pause)
		ret = 0.0f;
	else
		ret = App->GetDT();

	return ret;
}

float SceneManager::GetGameExecutionTime()
{
	return 0.0f;
}

