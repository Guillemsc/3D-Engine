#include "ResourcePrefabLoader.h"
#include "ResourcePrefab.h"
#include "App.h"

ResourcePrefabLoader::ResourcePrefabLoader():
ResourceLoader(ResourceType::RT_MESH, App->file_system->GetAssetsPath(), App->file_system->GetLibraryPrefabPath())
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

	std::string meta_path = (assets_path + decomposed_file_path.file_name + "." + decomposed_file_path.file_extension + ".meta");
	JSON_Doc* doc = App->json->CreateJSON(meta_path.c_str());
	if (doc != nullptr)
	{
		std::string uid = doc->GetString("uid", "no_uid");

		doc->Save();
		App->json->UnloadJSON(doc);
	}

	return ret;
}

bool ResourcePrefabLoader::UnloadAssetFromEngine(DecomposedFilePath decomposed_file_path)
{
	App->file_system->FileDelete(decomposed_file_path.file_path.c_str());
}

bool ResourcePrefabLoader::GetGameObjectFromPrefabPath(const char * filepath, GameObject *& go)
{
	bool ret = false;

	ClearRelations();

	JSON_Doc* prefab = App->json->LoadJSON(filepath);

	if (prefab != nullptr)
	{
		ret = true;

		// Load GameObjects
		int game_objects_count = prefab->GetArrayCount("GameObjects");
		for (int i = 0; i < game_objects_count; i++)
		{
			JSON_Doc go_node = prefab->GetJsonNode();
			go_node.MoveToSectionFromArray("GameObjects", i);

			int id = go_node.GetNumber("id", -1);
			int parent_id = go_node.GetNumber("parent_id", -1);
			GameObject* go = new GameObject();

			AddRelationIdGo(id, go, parent_id);

			if (go != nullptr)
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

void ResourcePrefabLoader::ClearRelations()
{
	relations.clear();
}

void ResourcePrefabLoader::AddRelationGo(GameObject * go)
{
	int id = relations.size();
	Relation rel(id, go);
	relations.push_back(rel);
}

void ResourcePrefabLoader::AddRelationIdGo(int id, GameObject * go, int parent_id)
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

int ResourcePrefabLoader::GetRelationGo(GameObject * go)
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

GameObject * ResourcePrefabLoader::GetRelationId(int id)
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

