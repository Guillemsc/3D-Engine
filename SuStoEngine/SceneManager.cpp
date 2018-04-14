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

	//App->resource_manager->ImportAllResources();

	//LoadTmpScene();
	//App->scene_manager->LoadScene(current_scene.c_str());

	return ret;
}

bool SceneManager::CleanUp()
{
	bool ret = true;

	App->scene_manager->SaveScene(current_scene.c_str());

	SaveTmpScene();

	return ret;
}

void SceneManager::SaveScene(const char * scene_name)
{
	//SavePrefab(scene_name, "scene", App->file_system->GetLibraryScenePath().c_str(), App->gameobj->GetRoot());
}

void SceneManager::LoadScene(const char * scene_name, bool set_scene_title)
{
	//DestroyScene();

	//string filepath = App->file_system->GetLibraryScenePath();
	//filepath += scene_name;
	//filepath += ".scene";

	//GameObject* go = nullptr;
	//LoadPrefab(filepath.c_str(), go);
}

void SceneManager::DestroyScene()
{
	App->gameobj->ClearSelection();
	App->gameobj->DestroyAllGameObjectsNow();
}

void SceneManager::SaveTmpScene()
{
	/*SaveScene("tmp_scene");*/
}

void SceneManager::LoadTmpScene()
{
	/*LoadScene("tmp_scene", false);*/
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

