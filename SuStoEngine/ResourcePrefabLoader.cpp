#include "ResourcePrefabLoader.h"
#include "ResourcePrefab.h"
#include "ResourceManager.h"
#include "GameObjectAbstractor.h"
#include "ModuleGameObject.h"
#include "App.h"

ResourcePrefabLoader::ResourcePrefabLoader():
ResourceLoader(ResourceType::RT_PREFAB, App->file_system->GetLibraryPrefabPath())
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

bool ResourcePrefabLoader::UnloadAssetFromEngine(DecomposedFilePath decomposed_file_path)
{
	App->file_system->FileDelete(decomposed_file_path.file_path.c_str());

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

	return true;
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

bool ResourcePrefabLoader::ImportResourceFromLibrary(const char * uid)
{
	bool ret = false;

	std::string prefab_path = library_path + uid + ".prefab";

	GameObjectAbstraction abs = App->gameobj->GetAbstractor()->DeSerialize(prefab_path.c_str());

	if (abs.GetValid())
	{
		ResourcePrefab* r_prefab = (ResourcePrefab*)App->resource_manager->CreateNewResource(ResourceType::RT_PREFAB);

		r_prefab->SetAbstraction(abs);

		ret = true;
	}

	return ret;
}

bool ResourcePrefabLoader::LoadAssetResourceIntoScene(Resource * resource)
{
	bool ret = false;

	if (resource != nullptr)
	{
		ResourcePrefab* rprefab = (ResourcePrefab*)resource;

		GameObjectAbstraction abs = rprefab->GetAbstraction();

		if (abs.GetValid())
		{
			rprefab->Instantiate();

			ret = true;
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
		r_prefab = (ResourcePrefab*)App->resource_manager->CreateNewResource(ResourceType::RT_PREFAB);
		
		r_prefab->SetGameObject(go);

		std::string uid = r_prefab->GetUniqueId();

		std::string name = go->GetName();

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

		App->gameobj->GetAbstractor()->Serialize(r_prefab->GetAbstraction(), library_path.c_str(), uid.c_str(), "prefab");
		
	}

	return r_prefab;
}
