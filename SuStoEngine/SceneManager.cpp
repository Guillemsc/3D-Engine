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
#include "SuStoUI.h"
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
	//App->scene_manager->LoadScene(current_scene.c_str());

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

	ClearRelations();

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

		for (vector<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end(); it++)
		{
			AddRelationGo((*it));
		}

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

				// Set relation id
				go_node.SetNumber("id", GetRelationGo((*it)));
				
				// Set parent relation id
				if((*it)->GetParent() != nullptr && (*it)->GetParent() != App->gameobj->GetRoot())
					go_node.SetNumber("parent_id", GetRelationGo((*it)->GetParent()));
				else
					go_node.SetNumber("parent_id", -1);

				(*it)->OnSaveSerialize(go_node);
			}
		}

		prefab->Save();

		ret = true;
	}

	ClearRelations();

	return ret;
}

bool SceneManager::LoadPrefab(const char * file_path, GameObject *& loaded_go)
{
	bool ret = false;

	string file_name = App->file_system->GetFileNameFromFilePath(file_path);

	LOG_OUTPUT("Loading prefab with name: %s", file_name.c_str());

	ClearRelations();

	JSON_Doc* prefab = App->json->LoadJSON(file_path);

	if (prefab != nullptr)
	{
		ret = true;

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

			int id = go_node.GetNumber("id", -1);
			int parent_id = go_node.GetNumber("parent_id", -1);
			GameObject* go = App->gameobj->Create();

			AddRleationIdGo(id, go, parent_id);

			if(go != nullptr)
				go->OnLoadSerialize(go_node);

			if (i == 0)
				loaded_go = go;
		}

		for (vector<Relation>::iterator it = relations.begin(); it != relations.end(); ++it)
		{
			GameObject* parent = GetRelationId((*it).id_parent);

			if (parent != nullptr)
				parent->AddChild((*it).go);
		}

		prefab->MoveToRoot();
	}

	ClearRelations();

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
		App->gameobj->GetUIMain()->SetUIMode(UIMode::UI_EDIT);
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
		App->gameobj->GetUIMain()->SetUIMode(UIMode::UI_PLAY);
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

void SceneManager::ClearRelations()
{
	relations.clear();
}

void SceneManager::AddRelationGo(GameObject * go)
{
	int id = relations.size();
	Relation rel(id, go);
	relations.push_back(rel);
}

void SceneManager::AddRleationIdGo(int id, GameObject * go, int parent_id)
{
	if (parent_id == -1)
	{
		Relation rel(id, go);
		relations.push_back(rel);
	}
	else
	{
		Relation rel(id, go, parent_id);
		relations.push_back(rel);
	}
}

int SceneManager::GetRelationGo(GameObject * go)
{
	for (vector<Relation>::iterator it = relations.begin(); it != relations.end(); ++it)
	{
		if ((*it).go == go)
		{
			return (*it).id;
		}
	}

	return -1;
}

GameObject * SceneManager::GetRelationId(int id)
{
	for (vector<Relation>::iterator it = relations.begin(); it != relations.end(); ++it)
	{
		if ((*it).id == id)
		{
			return (*it).go;
		}
	}

	return nullptr;
}

