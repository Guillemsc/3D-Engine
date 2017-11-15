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

	current_scene = "test.scene";

	App->resource_manager->ImportAllResources();
	App->scene_manager->LoadScene(current_scene.c_str());

	return ret;
}

bool SceneManager::CleanUp()
{
	bool ret = true;

	App->scene_manager->SaveScene("test.scene");

	return ret;
}

void SceneManager::SaveScene(const char * scene_name)
{
	LOG_OUTPUT("Saving scene %s", scene_name);

	string path = App->file_system->GetLibraryScenePath() + scene_name;

	JSON_Doc* scene = App->json->LoadJSON(path.c_str());

	if (scene == nullptr)
		scene = App->json->CreateJSON(path.c_str());

	if (scene != nullptr)
	{
		scene->Clear();

		vector<GameObject*> game_objects = App->gameobj->GetListGameObjects();

		scene->SetNumber3("editor_camera_position", App->camera->GetCurrentCamera()->GetPosition());
		scene->SetNumber3("editor_camera_front", App->camera->GetCurrentCamera()->GetZDir());
		scene->SetNumber3("editor_camera_up", App->camera->GetCurrentCamera()->GetYDir());

		// Store GameObjects
		scene->SetArray("GameObjects");
		scene->SetArray("Components");

		for (vector<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end(); it++)
		{
			scene->MoveToRoot();

			if((*it) != App->gameobj->GetRoot())
				(*it)->OnSaveScene(scene);
		}

		scene->Save();
	}
}

void SceneManager::LoadScene(const char * scene_name)
{
	LOG_OUTPUT("Loading scene %s", scene_name);

	// Load new scene
	string path = App->file_system->GetLibraryScenePath() + scene_name;

	JSON_Doc* scene = App->json->LoadJSON(path.c_str());

	if (scene != nullptr)
	{
		// Engine title --------------
		string title_name = App->GetAppName();
		title_name += " ";
		title_name += App->GetVersion();
		title_name += " - ";
		title_name += scene_name;
		App->window->SetTitle(title_name.c_str());
		// ---------------------------

		float3 cam_pos = scene->GetNumber3("editor_camera_position");
		float3 z_dir = scene->GetNumber3("editor_camera_front");
		float3 y_dir = scene->GetNumber3("editor_camera_up");
		App->camera->GetCurrentCamera()->SetPosition(cam_pos);
		App->camera->GetCurrentCamera()->SetZDir(z_dir);
		App->camera->GetCurrentCamera()->SetYDir(y_dir);

		// Load GameObjects
		int game_objects_count = scene->GetArrayCount("GameObjects");
		for (int i = 0; i < game_objects_count; i++)
		{
			scene->MoveToSectionFromArray("GameObjects", i);

			string id = scene->GetString("uid", "no_id");
			string name = scene->GetString("name", "missing_name");

			GameObject* go = App->gameobj->Create(id);
			go->SetName(name);

			scene->MoveToRoot();
		}

		for (int i = 0; i < game_objects_count; i++)
		{
			scene->MoveToSectionFromArray("GameObjects", i);

			string parent = scene->GetString("parent", "no_id");
			string id = scene->GetString("uid", "no_id");
			
			if (parent != "")
			{
				GameObject* go_parent = App->gameobj->Find(parent);
				GameObject* go = App->gameobj->Find(id);

				go_parent->AddChild(go);
			}

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

			if (owner != nullptr)
			{
				switch (type)
				{
				case TRANSFORM:
				{
					float3 position = scene->GetNumber3("position");
					float4 rotation = scene->GetNumber4("rotation");
					float3 scale = scene->GetNumber3("scale");

					owner->transform->ForceUid(component_id);

					owner->transform->SetPosition(position);
					owner->transform->SetRotation(Quat(rotation.x, rotation.y, rotation.w, rotation.z));
					owner->transform->SetScale(scale);
				}
				break;
				case MESH:
				{
					string mesh_id = scene->GetString("mesh_id", "no_id");

					owner->AddComponent(MESH, component_id);
					ComponentMesh* cmesh = (ComponentMesh*)owner->GetComponent(MESH);
					ResourceMesh* rmesh = (ResourceMesh*)App->resource_manager->Get(mesh_id);
					cmesh->SetMesh(rmesh);
				}
				break;
				case MATERIAL:
				{
					string texture_id = scene->GetString("texture_id", "no_id");

					owner->AddComponent(MATERIAL, component_id);
					ComponentMaterial* cmaterial = (ComponentMaterial*)owner->GetComponent(MATERIAL);
					ResourceTexture* rtexture = (ResourceTexture*)App->resource_manager->Get(texture_id);
					cmaterial->SetTexture(rtexture);
				}
				break;
				case CAMERA:
				{
					float far_plane_distance = scene->GetNumber("far_plane_distance", 1000);
					float near_plane_distance = scene->GetNumber("near_plane_distance", 0.1f);
					float fov = scene->GetNumber("fov", 60.0f);

					owner->AddComponent(CAMERA, component_id);
					ComponentCamera* ccamera = (ComponentCamera*)owner->GetComponent(CAMERA);
					ccamera->GetCamera()->SetFarPlaneDistance(far_plane_distance);
					ccamera->GetCamera()->SetNearPlaneDistance(near_plane_distance);
					ccamera->GetCamera()->SetFOV(fov);
				}
				break;
				}
			}

			scene->MoveToRoot();
		}
	}
}

void SceneManager::DestroyScene()
{
	App->gameobj->ClearSelection();
	App->gameobj->DestroyAllGameObjectsNow();
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

		DestroyScene();

		LoadScene(current_scene.c_str());
	}
}

void SceneManager::Play()
{
	if (state != PLAY)
	{
		state = PLAY;

		SaveScene(current_scene.c_str());
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
	return 0.0f;
}

float SceneManager::GetGameExecutionTime()
{
	return 0.0f;
}

