#include "ResourcePrefabLoader.h"
#include "ResourcePrefab.h"
#include "ResourceManager.h"
#include "GameObjectAbstractor.h"
#include "ModuleGameObject.h"
#include "App.h"

ResourcePrefabLoader::ResourcePrefabLoader():
ResourceLoader(ResourceType::RT_PREFAB, App->file_system->GetAssetsPath(), App->file_system->GetLibraryPrefabPath())
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


	return ret;
}

bool ResourcePrefabLoader::UnloadAssetFromEngine(DecomposedFilePath decomposed_file_path)
{
	App->file_system->FileDelete(decomposed_file_path.file_path.c_str());

	return true;
}

bool ResourcePrefabLoader::GetGameObjectFromPrefabPath(const char * filepath, GameObject *& go)
{
	bool ret = false;

	//ClearRelations();

	//JSON_Doc* prefab = App->json->LoadJSON(filepath);

	//if (prefab != nullptr)
	//{
	//	ret = true;

	//	// Load GameObjects
	//	int game_objects_count = prefab->GetArrayCount("GameObjects");
	//	for (int i = 0; i < game_objects_count; i++)
	//	{
	//		JSON_Doc go_node = prefab->GetJsonNode();
	//		go_node.MoveToSectionFromArray("GameObjects", i);

	//		int id = go_node.GetNumber("id", -1);
	//		int parent_id = go_node.GetNumber("parent_id", -1);

	//		AddRelationIdGo(id, go, parent_id);

	//		if (go != nullptr)
	//			go->OnLoadSerialize(go_node);

	//	}

	//	for (vector<Relation>::iterator it = relations.begin(); it != relations.end(); ++it)
	//	{
	//		GameObject* parent = GetRelationId((*it).id_parent);

	//		if (parent != nullptr)
	//			parent->AddChild((*it).go);
	//	}

	//	prefab->MoveToRoot();
	//}

	//ClearRelations();

	return ret;
}

ResourcePrefab * ResourcePrefabLoader::CreatePrefab(GameObject * go)
{
	ResourcePrefab* rprefab = (ResourcePrefab*)App->resource_manager->CreateNewResource(ResourceType::RT_PREFAB);

	GameObjectAbstraction abs = App->gameobj->GetAbstractor()->Abstract(go);

	rprefab->SetAbstraction(abs);

	App->gameobj->GetAbstractor()->DeAbstract(abs);

	App->gameobj->GetAbstractor()->Serialize(abs, "C:\\Users\\Guillem\\Documents\\GitHub\\3D-Engine\\", "test");
	App->gameobj->GetAbstractor()->DeSerialize("C:\\Users\\Guillem\\Documents\\GitHub\\3D-Engine\\test.json");

	return rprefab;
}

