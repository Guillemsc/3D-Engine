#include "ResourcePrefabLoader.h"
#include "ResourcePrefab.h"
#include "ResourceManager.h"
#include "GameObjectAbstractor.h"
#include "ModuleGameObject.h"
#include "App.h"

ResourcePrefabLoader::ResourcePrefabLoader():
ResourceLoader(ResourceType::RT_PREFAB, App->file_system->GetLibraryPrefabPath(), "PrefabLoader")
{
}

ResourcePrefabLoader::~ResourcePrefabLoader()
{
}

Resource * ResourcePrefabLoader::CreateResource(std::string new_uid)
{
	Resource* ret = nullptr;

	ret = new ResourcePrefab(new_uid);

	return ret;
}

bool ResourcePrefabLoader::LoadFileToEngine(DecomposedFilePath decomposed_file_path, std::vector<Resource*>& resources)
{
	bool ret = true;

	JSON_Doc* doc = App->json->LoadJSON(decomposed_file_path.file_path.c_str());

	if (doc != nullptr)
	{
		GameObjectAbstraction abs = App->gameobj->GetAbstractor()->DeSerialize(decomposed_file_path.file_path.c_str());

		ResourcePrefab* r_prefab = (ResourcePrefab*)App->resource_manager->CreateNewResource(ResourceType::RT_PREFAB);
		r_prefab->SetAbstraction(abs);

		std::string meta_filepath = decomposed_file_path.file_path + ".meta";

		if (App->file_system->FileExists(meta_filepath.c_str()))
			App->file_system->FileDelete(meta_filepath.c_str());

		JSON_Doc* meta_doc = App->json->CreateJSON(meta_filepath.c_str());
		if (meta_doc != nullptr)
		{
			meta_doc->SetString("resource", r_prefab->GetUniqueId().c_str());

			meta_doc->Save();

			App->json->UnloadJSON(meta_doc);

			ExportResourceToLibrary(r_prefab);
		}
	}

	return ret;
}

bool ResourcePrefabLoader::RemoveAssetInfoFromEngine(DecomposedFilePath decomposed_file_path)
{
	bool ret = false;

	std::string meta_filepath = decomposed_file_path.file_path + ".meta";

	JSON_Doc* meta_doc = App->json->LoadJSON(meta_filepath.c_str());
	if (meta_doc != nullptr)
	{
		std::string resource_uid = meta_doc->GetString("resource");

		std::string resource_filepath = library_path + resource_uid + ".prefab";

		App->file_system->FileDelete(resource_filepath.c_str());

		App->json->UnloadJSON(meta_doc);
	}

	App->file_system->FileDelete(meta_filepath.c_str());

	return ret;
}

void ResourcePrefabLoader::ClearFromGameObject(Resource * resource, GameObject * go)
{
}

bool ResourcePrefabLoader::ExportResourceToLibrary(Resource * resource)
{
	bool ret = false;

	if (resource != nullptr)
	{
		ResourcePrefab* rprefab = (ResourcePrefab*)resource;

		GameObjectAbstraction abs = rprefab->GetAbstraction();

		if (abs.GetValid())
		{
			App->gameobj->GetAbstractor()->Serialize(abs, library_path.c_str(), rprefab->GetUniqueId().c_str(), "prefab");

			ret = true;
		}
	}
	return ret;
}

bool ResourcePrefabLoader::ExportResourceToAssets(Resource * resource)
{
	bool ret = false;

	if (resource != nullptr)
	{
		ResourcePrefab* r_prefab = (ResourcePrefab*)resource;

		GameObjectAbstraction abs = r_prefab->GetAbstraction();

		if (abs.GetValid())
		{
			std::string name = r_prefab->GetFileName();

			name = App->file_system->FileRenameOnNameCollision(App->file_system->GetAssetsPath().c_str(), name.c_str(), "prefab");

			App->gameobj->GetAbstractor()->Serialize(r_prefab->GetAbstraction(), App->file_system->GetAssetsPath().c_str(), name.c_str(), "prefab");

			std::string meta_filepath = App->file_system->GetAssetsPath() + name + ".prefab" + ".meta";

			if (App->file_system->FileExists(meta_filepath.c_str()))
				App->file_system->FileDelete(meta_filepath.c_str());

			JSON_Doc* meta_doc = App->json->CreateJSON(meta_filepath.c_str());
			if (meta_doc != nullptr)
			{
				meta_doc->SetString("resource", r_prefab->GetUniqueId().c_str());

				meta_doc->Save();

				App->json->UnloadJSON(meta_doc);
			}
		}
	}

	return ret;
}

bool ResourcePrefabLoader::ImportResourceFromLibrary(DecomposedFilePath d_filepath)
{
	bool ret = false;

	GameObjectAbstraction abs = App->gameobj->GetAbstractor()->DeSerialize(d_filepath.file_path.c_str());

	if (abs.GetValid())
	{
		ResourcePrefab* r_prefab = (ResourcePrefab*)App->resource_manager->CreateNewResource(ResourceType::RT_PREFAB, d_filepath.file_name.c_str());

		r_prefab->SetAbstraction(abs);

		ret = true;
	}

	return ret;
}

bool ResourcePrefabLoader::LoadAssetIntoScene(DecomposedFilePath d_filepath)
{
	bool ret = false;

	std::string meta_path = d_filepath.file_path + ".meta";

	if (App->file_system->FileExists(meta_path.c_str()))
	{
		JSON_Doc* doc = App->json->LoadJSON(meta_path.c_str());
		if (doc != nullptr)
		{
			std::string uid = doc->GetString("resource");

			std::string resource_path = library_path + uid + ".prefab";

			GameObjectAbstraction abs = App->gameobj->GetAbstractor()->DeSerialize(resource_path.c_str());

			if (abs.GetValid())
			{
				App->gameobj->GetAbstractor()->DeAbstract(abs);
			}
		}
	}

	return ret;
}

bool ResourcePrefabLoader::IsAssetOnLibrary(DecomposedFilePath d_filepath, std::vector<std::string>& library_files_used)
{
	bool ret = false;

	std::string meta_file = d_filepath.file_path + ".meta";

	if (App->file_system->FileExists(meta_file.c_str()))
	{
		JSON_Doc* doc = App->json->LoadJSON(meta_file.c_str());
		if (doc != nullptr)
		{
			std::string uid = doc->GetString("resource");

			std::string resource_path = library_path + uid + ".prefab";

			if (App->file_system->FileExists(resource_path.c_str()))
			{
				library_files_used.push_back(resource_path);

				ret = true;
			}
		}
	}

	return ret;
}

bool ResourcePrefabLoader::RenameAsset(DecomposedFilePath decomposed_file_path, const char * new_name)
{
	return false;
}

ResourcePrefab * ResourcePrefabLoader::CreatePrefab(GameObject * go)
{
	ResourcePrefab* r_prefab = nullptr;

	if (go != nullptr)
	{
		std::string name = go->GetName();

		r_prefab = (ResourcePrefab*)App->resource_manager->CreateNewResource(ResourceType::RT_PREFAB);
		
		r_prefab->SetGameObject(go);

		r_prefab->SetFileName(name.c_str());

		ExportResourceToAssets(r_prefab);
		ExportResourceToLibrary(r_prefab);
	}

	return r_prefab;
}

void ResourcePrefabLoader::UpdatePrefab(const char* prefab_filepath, GameObject* go)
{
	if (go != nullptr)
	{
		DecomposedFilePath d_filepath = App->file_system->DecomposeFilePath(prefab_filepath);

		std::string meta_path = std::string(prefab_filepath) + ".meta";

		JSON_Doc* doc = App->json->LoadJSON(meta_path.c_str());
		if (doc != nullptr)
		{
			std::string uid = doc->GetString("resource");

			ResourcePrefab* r_prefab = (ResourcePrefab*)App->resource_manager->Get(uid);

			if (r_prefab != nullptr)
			{
				GameObjectAbstraction abs = r_prefab->GetAbstraction();

				if (abs.GetValid())
				{
					std::string resource_filepath = library_path + uid + ".prefab";

					App->file_system->FileDelete(prefab_filepath);
					App->file_system->FileDelete(resource_filepath.c_str());

					App->gameobj->GetAbstractor()->Serialize(abs, d_filepath.path.c_str(), d_filepath.file_name.c_str(), "prefab");
					App->gameobj->GetAbstractor()->Serialize(abs, library_path.c_str(), uid.c_str(), "prefab");
				}
			}

			App->json->UnloadJSON(doc);
		}
	}
}

