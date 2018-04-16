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

	App->file_system->FileCopyPaste(decomposed_file_path.path.c_str(), library_path.c_str(), true);

	string meta_name = decomposed_file_path.file_path + ".meta";
	JSON_Doc* doc = App->json->LoadJSON(meta_name.c_str());

	//if (doc == nullptr)
	//	doc = App->json->CreateJSON(meta_name.c_str());

	//if (doc != nullptr)
	//{
	//	doc->Clear();

	//	doc->SetString("uid", mesh->GetUniqueId().c_str());
	//	doc->SetString("name", mesh->GetFileName().c_str());

	//	doc->Save();
	//}

	//App->json->UnloadJSON(doc);

	return ret;
}

bool ResourcePrefabLoader::UnloadAssetFromEngine(DecomposedFilePath decomposed_file_path)
{
	App->file_system->FileDelete(decomposed_file_path.file_path.c_str());

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

