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

		ResourcePrefab* rprefab = (ResourcePrefab*)App->resource_manager->CreateNewResource(ResourceType::RT_PREFAB);
		rprefab->SetAbstraction(abs);


		std::string meta_filepath = decomposed_file_path.file_path + ".meta";

		JSON_Doc* meta_doc = App->json->CreateJSON(meta_filepath.c_str());
		if (meta_doc != nullptr)
		{
			meta_doc->SetString("resource", rprefab->GetUniqueId().c_str());

			ExportToLibrary(rprefab);
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

bool ResourcePrefabLoader::ExportToLibrary(Resource * resource)
{
	bool ret = true;

	ResourcePrefab* rprefab = (ResourcePrefab*)resource;

	GameObjectAbstraction abs = rprefab->GetAbstraction();

	if (abs.GetValid())
	{
		App->gameobj->GetAbstractor()->Serialize(abs, library_path.c_str(), rprefab->GetUniqueId().c_str(), "prefab");
	}

	return ret;
}

bool ResourcePrefabLoader::LoadIntoScene(Resource * resource)
{
	ResourcePrefab* rprefab = (ResourcePrefab*)resource;

	GameObjectAbstraction abs = rprefab->GetAbstraction();

	rprefab->Instantiate();

	return true;
}

ResourcePrefab * ResourcePrefabLoader::CreatePrefab(GameObject * go)
{
	ResourcePrefab* rprefab = nullptr;

	if (go != nullptr)
	{
		rprefab = (ResourcePrefab*)App->resource_manager->CreateNewResource(ResourceType::RT_PREFAB);

		if (rprefab != nullptr)
		{
			rprefab->SetGameObject(go);
		}
	}

	return rprefab;
}

