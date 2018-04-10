#ifndef __RESOURCE_PREFAB_LOADER_H__
#define __RESOURCE_PREFAB_LOADER_H__

#include "ResourceLoader.h"
#include "Resource.h"
#include <vector>

class ResourcePrefab;

struct Relation
{
	Relation(int _id, GameObject* _go)
	{
		id = _id;
		go = _go;
		id_parent = -1;
	}
	Relation(int _id, GameObject* _go, int _id_parent)
	{
		id = _id;
		go = _go;
		id_parent = _id_parent;
	}

	int id = 0;
	GameObject* go = nullptr;
	int id_parent = 0;
};

class ResourcePrefabLoader : public ResourceLoader
{
public:
	ResourcePrefabLoader();
	virtual ~ResourcePrefabLoader();

	Resource* CreateResource(std::string new_uid);

	bool LoadFileToEngine(DecomposedFilePath decomposed_file_path, std::vector<Resource*>& resources);
	bool UnloadAssetFromEngine(DecomposedFilePath decomposed_file_path);

	//void ClearFromGameObject(Resource* resource, GameObject* go);

	//bool ExportToLibrary(Resource* resource);
	//bool ImportFromLibrary(const char* uid);

	//bool LoadIntoScene(Resource* resource);

	//bool IsResourceOnLibrary(Resource* resource);
	//bool IsResourceOnAssets(Resource* resource);

	//void CreateResourcesMissingOnAssets();
	//void RemoveResourcesMissingOnLibrary();
	// ---

	//bool Load(const char* filepath, std::vector<Resource*>& resources);
	//void Import(const char* filepath);
	//bool Export(const char* filepath, ResourceTexture* resource);
	//void ImportAllTextures();

	//void Unload(const char* filepath);
	//
	//TextureInfo LoadTexture(const char * filename, bool avoid_flip = false);
	//void UnloadTexture(unsigned int id);

	bool GetGameObjectFromPrefabPath(const char* filepath, GameObject*& go);
	ResourcePrefab* CreateNewPrefabFromGameObject(GameObject* go);

private:
	void ClearRelations();
	void AddRelationGo(GameObject* go);
	void AddRelationIdGo(int id, GameObject* go, int parent_id = -1);
	int GetRelationGo(GameObject* go);
	GameObject* GetRelationId(int id);

private:
	std::vector<Relation> relations;

};

#endif